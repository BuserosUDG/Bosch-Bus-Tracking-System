/* own header files */
#include "XdkAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* own header files */
#include "AppController.h"

#include "SensorParameters.h"
#include "Select_Sensor.h"
#include "SensorsController.h"
#include "Controllers/AccelerometerController.h"
#include "Controllers/HumidityController.h"
#include "Controllers/PressureController.h"
#include "Controllers/TemperatureController.h"
#include "Controllers/MagnetometerController.h"
#include "Controllers/GyroscopeController.h"
#include "Controllers/LightController.h"
#include "Controllers/NoiseController.h"
#include "APIController.h"
#include "BTController.h"
#include "XDK_LED.h"

/* system header files */
#include <stdio.h>
#include <math.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"

Retcode_T statusWifi = RETCODE_OK;

static CmdProcessor_T *AppCmdProcessor;/**< Handle to store the main Command processor handle to be reused by ServalPAL thread */


static xTaskHandle AppControllerHandle = NULL;/**< OS thread handle for Application controller to be used by run-time blocking threads */


static void AppControllerFire(void* pvParameters)
{
    BCDS_UNUSED(pvParameters);

    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;
    SetProcessor(AppCmdProcessor);
    memset(&sensorValue, 0x00, sizeof(sensorValue));
    while (1)
    {

        retcode = Sensor_GetData(&sensorValue);
        if (RETCODE_OK == retcode)
        {
        	if(RETCODE_OK == statusWifi)
        	{
        		sendAPIData(&sensorValue);
        	}
        	setBTData(&sensorValue);
        }
        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }
        vTaskDelay(pdMS_TO_TICKS(APP_CONTROLLER_TX_DELAY));
    }
}


static void AppControllerEnable(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = Sensor_Enable();
    Retcode_T retcode2 = WLAN_Enable();
    Retcode_T retcode3 = BLE_Enable();
        if (RETCODE_OK == retcode2)
        {
            retcode2 = ServalPAL_Enable();
        }
        if (RETCODE_OK == retcode2)
        {
            retcode2 = HTTPRestClient_Enable();
        }
        if (RETCODE_OK != retcode2)
                   {
            	LED_On(LED_INBUILT_RED);
            	statusWifi = retcode2;
            	retcode2 = RETCODE_OK;
                   }
    if (RETCODE_OK == retcode && RETCODE_OK == retcode2 && RETCODE_OK == retcode3)
    {
        if (pdPASS != xTaskCreate(AppControllerFire, (const char * const ) "AppController", TASK_STACK_SIZE_APP_CONTROLLER, NULL, TASK_PRIO_APP_CONTROLLER, &AppControllerHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerEnable : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}


static void AppControllerSetup(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    SensorSetup.CmdProcessorHandle = AppCmdProcessor;
    Retcode_T retcode = Sensor_Setup(&SensorSetup);
    Retcode_T retcode2 = WLAN_Setup(&WLANSetupInfo);
    Retcode_T retcode3 = BLE_Setup(&BLESetupInfo);
    if (RETCODE_OK == retcode2)
       {
           retcode2 = ServalPAL_Setup(AppCmdProcessor);
       }
       if (RETCODE_OK == retcode2)
       {
           retcode2 = HTTPRestClient_Setup(&HTTPRestClientSetupInfo);
       }
    if (RETCODE_OK == retcode && RETCODE_OK == retcode2 && RETCODE_OK == retcode3)
    {
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerEnable, NULL, UINT32_C(0));
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerSetup : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}


void AppController_Init(void * cmdProcessorHandle, uint32_t param2)
{
    BCDS_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    if (cmdProcessorHandle == NULL)
    {
        printf("AppController_Init : Command processor handle is NULL \r\n");
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        AppCmdProcessor = (CmdProcessor_T *) cmdProcessorHandle;
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerSetup, NULL, UINT32_C(0));
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}

/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development. 
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/

/**
 * @ingroup APPS_LIST
 *
 * @defgroup STREAM_SENSOR_DATA_OVER_USB SensorData
 * @{
 *
 * @brief Application of printing all the defined sensors on serialport
 *
 * @details Demo application of printing all the defined sensors on serialport(USB virtual comport)
 *          every configured interval (#APP_CONTROLLER_TX_DELAY)
 *
 * @file
 **/

/* module includes ********************************************************** */

/* own header files */
#include "XdkAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* own header files */
#include "AppController.h"

/* system header files */
#include <stdio.h>
#include <math.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

static CmdProcessor_T *AppCmdProcessor;/**< Handle to store the main Command processor handle to be reused by ServalPAL thread */

static xTaskHandle AppControllerHandle = NULL;/**< OS thread handle for Application controller to be used by run-time blocking threads */

/**
 * @brief Callback function called when interrupt occurs
 *
 * @param [in]  param1
 * Unused
 *
 * @param [in]  param2
 * Unused
 */
static void AccelAppCallback(void *param1, uint32_t param2);

/**
 * @brief Callback function called when interrupt occurs
 *
 * @param [in]  param1
 * Unused
 *
 * @param [in]  param2
 * Unused
 */
static void LightAppCallback(void *param1, uint32_t param2);

static Sensor_Setup_T SensorSetup =    			// EN ESTA FUNCION SE INICIAN LOS SENSORES
        {
                .CmdProcessorHandle = NULL,
                .Enable =						// AQUI PODEMOS ELEGIR EL SENSOR QUE QUEREMOS QUE ENVIE VALORES
                        {
                                .Accel = true,
                                .Mag = true,
                                .Gyro = true,
                                .Humidity = true,
                                .Temp = true,
                                .Pressure = true,
                                .Light = true,
                                .Noise = true,
                        },
                .Config =
                        {
                                .Accel =
                                        {
                                                .Type = SENSOR_ACCEL_BMA280,
                                                .IsRawData = false,
                                                .IsInteruptEnabled = true,
                                                .Callback = AccelAppCallback,
                                        },
                                .Gyro =
                                        {
                                                .Type = SENSOR_GYRO_BMG160,
                                                .IsRawData = false,
                                        },
                                .Mag =
                                        {
                                                .IsRawData = false,
                                        },
                                .Light =
                                        {
                                                .IsInteruptEnabled = true,
                                                .Callback = LightAppCallback,
                                        },
                                .Temp =
                                        {
                                                .OffsetCorrection = APP_TEMPERATURE_OFFSET_CORRECTION,
                                        },
                        },
        };/**< Sensor setup parameters */

/* global variables ********************************************************* */

float gravity = 9.81;
float mgtogx;
float mgtogy;
float mgtogz;
float mtsx;
float mtsy;
float mtsz;

float temp;

float luxes;

float vpp;
float db;

static void AccelAppCallback(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    //printf("Accelerometer sensor application callback received\r\n");
}

static void LightAppCallback(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    //printf("*******Light sensor application callback received*******\r\n");
}

/**
 * @brief This function controls the application flow
 * - Triggers Sensor data sampling
 * - Read the sampled Sensor data
 *
 * @param[in] pvParameters
 * Unused
 */
static void AppControllerFire(void* pvParameters)     // ESTA FUNCION MUESTRA LOS DATOS OBTENIDOS DE LOS SENSORES
{
    BCDS_UNUSED(pvParameters);

    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));
    while (1)
    {

        retcode = Sensor_GetData(&sensorValue);
        if (RETCODE_OK == retcode)
        {
            if (SensorSetup.Enable.Accel && !(SensorSetup.Config.Accel.IsRawData))
            {
                printf("Accelerometer Converted data :\n\rx =%ld mg\n\ry =%ld mg\n\rz =%ld mg\r\n",
                        (long int) sensorValue.Accel.X,
                        (long int) sensorValue.Accel.Y,
                        (long int) sensorValue.Accel.Z);

                printf("\ \r\n");

            	mgtogx =  (float) sensorValue.Accel.X / 1000;
                mgtogy =  (float) sensorValue.Accel.Y / 1000;
                mgtogz =  (float) sensorValue.Accel.Z / 1000;

                mtsx = mgtogx * gravity - 0.3;
                mtsy = mgtogy * gravity - 0.3;
                mtsz = mgtogz * gravity - 0.3;

            	printf("Valores en m/s2 :\n\rx =%0.3f m/s2\n\ry =%0.3f m/s2\n\rz =%0.3f m/s2\r\n",
            			(float) mtsx, (float) mtsy, (float) mtsz);

            	printf("\ \r\n");

            }
            if (SensorSetup.Enable.Accel && (SensorSetup.Config.Accel.IsRawData))
            {
                printf("Accelerometer Raw data :\n\rx =%ld \n\ry =%ld\n\rz =%ld\r\n",
                        (long int) sensorValue.Accel.X,
                        (long int) sensorValue.Accel.Y,
                        (long int) sensorValue.Accel.Z);

                printf("\ \r\n");

            	mgtogx =  (float) sensorValue.Accel.X / 1000;
                mgtogy =  (float) sensorValue.Accel.Y / 1000;
                mgtogz =  (float) sensorValue.Accel.Z / 1000;

                mtsx = mgtogx * gravity - 0.3;
                mtsy = mgtogy * gravity - 0.3;
                mtsz = mgtogz * gravity - 0.3;

            	printf("Valores en m/s2 :\n\rx =%0.3f m/s2\n\ry =%0.3f m/s2\n\rz =%0.3f m/s2\r\n",
            			(float) mtsx, (float) mtsy, (float) mtsz);

            	printf("\ \r\n");

            }
            if (SensorSetup.Enable.Humidity)
            {
                printf("BME280 Environmental Conversion Data for Humidity:\n\rh =%ld %%rh\r\n",
                        (long int) sensorValue.RH);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Pressure)
            {
                printf("BME280 Environmental Conversion Data for Pressure :\n\rp =%ld Pa\r\n",
                        (long int) sensorValue.Pressure);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Temp)
            {
                printf("BME280 Environmental Conversion Data for temperature :\n\rt =%ld mDeg\r\n",
                        (long int) sensorValue.Temp);

                printf("\ \r\n");

            	temp = ((float) sensorValue.Temp / 1000);

            	printf("Valor de temperatura :\n\rt =%0.2f C\r\n",temp);

            	printf("\ \r\n");

            }
            if (SensorSetup.Enable.Mag && !(SensorSetup.Config.Mag.IsRawData))
            {
                printf("Magnetometer Converted data :\n\rx =%ld microTesla\n\ry =%ld microTesla\n\rz =%ld microTesla\n\rr =%ld\r\n",
                        (long int) sensorValue.Mag.X,
                        (long int) sensorValue.Mag.Y,
                        (long int) sensorValue.Mag.Z,
                        (long int) sensorValue.Mag.R);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Mag && SensorSetup.Config.Mag.IsRawData)
            {
                printf("Magnetometer Raw data :\n\rx =%ld\n\ry =%ld\n\rz =%ld \n\rr =%ld\r\n",
                        (long int) sensorValue.Mag.X,
                        (long int) sensorValue.Mag.Y,
                        (long int) sensorValue.Mag.Z,
                        (long int) sensorValue.Mag.R);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Gyro && !(SensorSetup.Config.Gyro.IsRawData))
            {
                printf("Gyro Converted Data :\n\rx =%ld mDeg\n\ry =%ld mDeg\n\rz =%ld mDeg\r\n",
                        (long int) sensorValue.Gyro.X,
                        (long int) sensorValue.Gyro.Y,
                        (long int) sensorValue.Gyro.Z);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Gyro && SensorSetup.Config.Gyro.IsRawData)
            {
                printf("Gyro Raw Data :\n\rx =%ld \n\ry =%ld \n\rz =%ld \r\n",
                        (long int) sensorValue.Gyro.X,
                        (long int) sensorValue.Gyro.Y,
                        (long int) sensorValue.Gyro.Z);

                printf("\ \r\n");
            }
            if (SensorSetup.Enable.Light)
            {
                printf("Light sensor data obtained in millilux :%d \n\r", (unsigned int) sensorValue.Light);

                printf("\ \r\n");

            	luxes = ((float) sensorValue.Light / 1000);

            	printf("Valor en luxes: %f lux\n\r", luxes);

            	printf("\ \r\n");

            }
            if (SensorSetup.Enable.Noise)
            {
                printf("Noise Sensor RMS Voltage :\r\nVrms = %f \r\n", sensorValue.Noise);

                printf("\ \r\n");

            	vpp = 2 * sensorValue.Noise;

                db = -20 * log10f(vpp / 0.707);

            	printf("Noise Sensor data:\r\ndb = %f \r\n", db);

            	printf("\ \r\n");
            	printf("\ \r\n");
            }
        }
        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }
        vTaskDelay(pdMS_TO_TICKS(APP_CONTROLLER_TX_DELAY));
    }
}

/**
 * @brief To enable the necessary modules for the application
 * - Sensor
 *
 * @param[in] param1
 * Unused
 *
 * @param[in] param2
 * Unused
 */
static void AppControllerEnable(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = Sensor_Enable();
    if (RETCODE_OK == retcode)
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

/**
 * @brief To setup the necessary modules for the application
 * - Sensor
 *
 * @param[in] param1
 * Unused
 *
 * @param[in] param2
 * Unused
 */
static void AppControllerSetup(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    SensorSetup.CmdProcessorHandle = AppCmdProcessor;
    Retcode_T retcode = Sensor_Setup(&SensorSetup);
    if (RETCODE_OK == retcode)
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

/* global functions ********************************************************** */

/** Refer interface header for description */
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

/**@} */
/** ************************************************************************* */

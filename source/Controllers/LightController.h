#ifndef SOURCE_GET_LIGHT_H_
#define SOURCE_GET_LIGHT_H_

#include "../Select_Sensor.h"
#include "XDK_Utils.h"

#include <stdio.h>
#include <math.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../SensorParameters.h"


get_Light()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

        if (SensorSetup.Enable.Light)
        {
        	printf("Light sensor data obtained in millilux :%d \n\r", (unsigned int) sensorValue.Light);

        	printf("\ \r\n");

        	luxes = ((float) sensorValue.Light / 1000);
        	printf("Valor en luxes: %f lux\n\r", luxes);

        	printf("\ \r\n");
        }
    }
}

#endif /* SOURCE_GET_LIGHT_H_ */

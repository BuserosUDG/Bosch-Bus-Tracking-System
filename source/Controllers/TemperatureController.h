#ifndef SOURCE_GET_TEMPERATURE_H_
#define SOURCE_GET_TEMPERATURE_H_

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


get_Temperature()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

        if (SensorSetup.Enable.Temp)
        {
        	printf("BME280 Environmental Conversion Data for temperature :\n\rt =%.2f mD\r\n",
        			(float) sensorValue.Temp);

        	printf("\ \r\n");

        	temp = ((float) sensorValue.Temp / 1000);

        	printf("Valor de temperatura :\n\rt =%0.2f C\r\n",temp);

        	printf("\ \r\n");

        }

    }
}

#endif /* SOURCE_GET_TEMPERATURE_H_ */

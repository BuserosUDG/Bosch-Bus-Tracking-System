#ifndef SOURCE_GET_PRESSURE_H_
#define SOURCE_GET_PRESSURE_H_

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


get_Pressure()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

        if (SensorSetup.Enable.Pressure)
        {
            printf("BME280 Environmental Conversion Data for Pressure :\n\rp =%ld Pa\r\n",
                    (long int) sensorValue.Pressure);

            printf("\ \r\n");

        }

    }
}

#endif /* SOURCE_GET_PRESSURE_H_ */

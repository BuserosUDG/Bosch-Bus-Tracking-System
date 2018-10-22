#ifndef SOURCE_GET_MAGNETOMETER_H_
#define SOURCE_GET_MAGNETOMETER_H_

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


get_Magnetometer()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

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

    }
}

#endif /* SOURCE_GET_MAGNETOMETER_H_ */

#ifndef SOURCE_GET_GYRO_H_
#define SOURCE_GET_GYRO_H_

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


get_Gyroscope()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

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
    }
}


#endif /* SOURCE_GET_GYRO_H_ */

#ifndef SOURCE_GET_NOISE_H_
#define SOURCE_GET_NOISE_H_

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


get_Noise()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

    retcode = Sensor_GetData(&sensorValue);
    if (RETCODE_OK == retcode)
    {

        if (SensorSetup.Enable.Noise)
        {
        	printf("Noise Sensor RMS Voltage :\r\nVrms = %f \r\n", sensorValue.Noise);

        	printf("\ \r\n");

        	vpp = 2 * sensorValue.Noise;
            db = -20 * log10f(vpp / 0.707);
        	printf("Noise Sensor data:\r\ndb = %f \r\n", db);

        	printf("\ \r\n");
        }
    }
}

#endif /* SOURCE_GET_NOISE_H_ */

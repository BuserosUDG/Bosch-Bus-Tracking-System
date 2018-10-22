#ifndef SOURCE_GET_ACCELEROMETER_H_
#define SOURCE_GET_ACCELEROMETER_H_

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

float gravity1 = 9.81;
float mgtogx;
float mgtogy;
float mgtogz;
float mtsx;
float mtsy;
float mtsz;


get_Accelerometer()
{
    Retcode_T retcode = RETCODE_OK;
    Sensor_Value_T sensorValue;

    memset(&sensorValue, 0x00, sizeof(sensorValue));

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

	                mtsx = mgtogx * gravity1 - 0.3;
	                mtsy = mgtogy * gravity1 - 0.3;
	                mtsz = mgtogz * gravity1 - 0.3;

	            	printf("Valores en m/s2 :\n\rx =%0.3f m/s2\n\ry =%0.3f m/s2\n\rz =%0.3f m/s2\r\n",
	            			(float) mtsx, (float) mtsy, (float) mtsz);

	            	printf("\ \r\n");
	            }

	            if (SensorSetup.Enable.Accel && (SensorSetup.Config.Accel.IsRawData))
	            {
	                printf("Accelerometer Converted data :\n\rx =%ld mg\n\ry =%ld mg\n\rz =%ld mg\r\n",
	                        (long int) sensorValue.Accel.X,
	                        (long int) sensorValue.Accel.Y,
	                        (long int) sensorValue.Accel.Z);

	                printf("\ \r\n");

	           	   mgtogx =  (float) sensorValue.Accel.X / 1000;
	               mgtogy =  (float) sensorValue.Accel.Y / 1000;
	               mgtogz =  (float) sensorValue.Accel.Z / 1000;

	               mtsx = mgtogx * gravity1 - 0.3;
	               mtsy = mgtogy * gravity1 - 0.3;
	               mtsz = mgtogz * gravity1 - 0.3;

	               printf("Valores en m/s2 :\n\rx =%0.3f m/s2\n\ry =%0.3f m/s2\n\rz =%0.3f m/s2\r\n",
	           			(float) mtsx, (float) mtsy, (float) mtsz);

	               printf("\ \r\n");

	            }
    }
}

#endif /* SOURCE_GET_ACCELEROMETER_H_ */

#ifndef SOURCE_ALLSENSORSCONTROLLER_H_
#define SOURCE_ALLSENSORSCONTROLLER_H_

#include "Select_Sensor.h"
#include "XDK_Utils.h"

#include <stdio.h>
#include <math.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SensorParameters.h"

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

getAllSensorsData()
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

	                mtsx = mgtogx * gravity - 0.3;
	                mtsy = mgtogy * gravity - 0.3;
	                mtsz = mgtogz * gravity - 0.3;

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
	            	printf("BME280 Environmental Conversion Data for temperature :\n\rt =%.2f mD\r\n",
	            			(float) sensorValue.Temp);

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
	            }

    }

}

#endif /* SOURCE_ALLSENSORSCONTROLLER_H_ */

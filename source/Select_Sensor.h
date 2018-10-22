#ifndef SOURCE_SELECT_SENSOR_H_
#define SOURCE_SELECT_SENSOR_H_

/* system header files */
//#include "AppController.h"
#include <stdio.h>
#include <math.h>

/* additional interface header files */
#include "XDK_Sensor.h"
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SensorParameters.h"

static void AccelAppCallback(void *param1, uint32_t param2);

static void LightAppCallback(void *param1, uint32_t param2);

static Sensor_Setup_T SensorSetup =
        {
                .CmdProcessorHandle = NULL,
                .Enable =
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

static void AccelAppCallback(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);
}

static void LightAppCallback(void *param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);
}

#endif /* SOURCE_SELECT_SENSOR_H_ */

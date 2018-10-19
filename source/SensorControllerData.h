/*
 * SensorControllerData.h
 *
 *  Created on: 19/10/2018
 *      Author: CAB1GA
 */

#ifndef SOURCE_SENSORCONTROLLERDATA_H_
#define SOURCE_SENSORCONTROLLERDATA_H_

/* local interface declaration ********************************************** */
#include "XDK_Utils.h"

/* local type and macro definitions */

/* local module global variable declarations */

/* local inline function definitions */

/* Macro to convert readable representation of IPv4 in terms of uint32_t variable */
/**
 *  * APP_CONTROLLER_TX_DELAY is sensor data transmission in milliseconds
 */
#define APP_CONTROLLER_TX_DELAY             UINT32_C(10000)

/**
 * APP_TEMPERATURE_OFFSET_CORRECTION is the Temperature sensor offset correction value (in mDegC).
 * Unused if APP_SENSOR_TEMP is false.
 * This is the Self heating, temperature correction factor.
 */
#define APP_TEMPERATURE_OFFSET_CORRECTION   (-3459)

/**
 * APP_CURRENT_RATED_TRANSFORMATION_RATIO is the current rated transformation ratio.
 * Unused if APP_SENSOR_CURRENT is false.
 * This will vary from one external LEM sensor to another.
 */
#define APP_CURRENT_RATED_TRANSFORMATION_RATIO      (0)

#endif /* SOURCE_SENSORCONTROLLERDATA_H_ */

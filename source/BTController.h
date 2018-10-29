#ifndef SOURCE_BTCONTROLLER_H_
#define SOURCE_BTCONTROLLER_H_

#include "XdkAppInfo.h"

/* additional interface header files */
#include "XDK_BLE.h"


/* constant definitions ***************************************************** */

/**
 * APP_CONTROLLER_BLE_DEVICE_NAME is the BLE device name.
 */
#define APP_CONTROLLER_BLE_DEVICE_NAME     "BOSCHDAY_XDK_01"

/**
 * APP_CONTROLLER_BLE_START_ID is the part of the payload to be received from
 * BLE central to start streaming data via. BLE.
 */
#define APP_CONTROLLER_BLE_START_ID         "getdata"

#define BT_COMMAND_GET_TEMP 				"gettemp"
#define BT_COMMAND_GET_ACCEL 				"getaccel"
#define BT_COMMAND_GET_MAG 					"getmag"
#define BT_COMMAND_GET_GYRO 				"getgyro"
#define BT_COMMAND_GET_HUM	 				"gethum"
#define BT_COMMAND_GET_PRESS 				"getpress"
#define BT_COMMAND_GET_LIGHT 				"getlight"
#define BT_COMMAND_GET_NOISE 				"getnoise"

enum BT_COMMAND_ID {
	BT_COMMAND_GET_TEMP_ID,
	BT_COMMAND_GET_ACCEL_ID,
	BT_COMMAND_GET_MAG_ID,
	BT_COMMAND_GET_GYRO_ID,
	BT_COMMAND_GET_HUM_ID,
	BT_COMMAND_GET_PRESS_ID,
	BT_COMMAND_GET_LIGHT_ID,
	BT_COMMAND_GET_NOISE_ID,};

/**
 * APP_CONTROLLER_BLE_END_ID is the part of the payload to be received from
 * BLE central to stop streaming data via. BLE.
 */
#define APP_CONTROLLER_BLE_END_ID           "end"

/**
 * APP_CONTROLLER_BLE_START_PAYLOAD is the first payload to be sent to the
 * BLE central upon successful reception of APP_CONTROLLER_BLE_START_ID.
 * Must not be more than 20 bytes.
 */
#define APP_CONTROLLER_BLE_START_PAYLOAD    "X      Y      Z"

/**
 * APP_CONTROLLER_BLE_END_PAYLOAD is the last payload to be sent to the
 * BLE central upon successful reception of APP_CONTROLLER_BLE_END_ID.
 * Must not be more than 20 bytes.
 */
#define APP_CONTROLLER_BLE_END_PAYLOAD      "Transfer Terminated!"

#define APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS       UINT32_C(1000)/**< Timeout for BLE send */

#define APP_CONTROLLER_BLE_START_TRIGGER            UINT32_C(1)/**< BLE start command send application logic */

#define APP_CONTROLLER_BLE_END_TRIGGER              UINT8_C(0)/**< BLE end command send application logic */

#define APP_CONTROLLER_BLE_TX_LEN                   UINT8_C(16)/**< Size of accelerometer complete payload in bytes with spaces in-between the axis values and NULL terminator */

#define APP_CONTROLLER_TX_AXIS_COUNT                UINT8_C(3)/**< Number of accelerometer axis (x, y & z) */

#define APP_TEMPERATURE_OFFSET_CORRECTION               (-3459)/**< Macro for static temperature offset correction. Self heating, temperature correction factor */

#define APP_CONTROLLER_BLE_RX_BUFFER_SIZE          UINT8_C(20)/**< Size of BLE receive buffer*/

static bool AppControllerBleTransmitPayload = false; /**< Boolean representing if BLE needs to be streamed. Validate the repeated start flag */

static void AppControllerBleDataRxCB(uint8_t *rxBuffer, uint8_t rxDataLength, void * param);

static BLE_Setup_T BLESetupInfo =
{
		.DeviceName = APP_CONTROLLER_BLE_DEVICE_NAME,
		.IsMacAddrConfigured = false,
		.MacAddr = 0UL,
		.Service = BLE_BCDS_BIDIRECTIONAL_SERVICE,
		.IsDeviceCharacteristicEnabled = false,
		.CharacteristicValue =
		{
				.ModelNumber = NULL,
				.Manufacturer = NULL,
				.SoftwareRevision = NULL
		},
		.DataRxCB = AppControllerBleDataRxCB,
		.CustomServiceRegistryCB = NULL,
};/**< BLE setup parameters */


Sensor_Value_T latestSensorValue;
CmdProcessor_T * CmdProcessorHandle;

static uint8_t getCommandId(uint8_t* bleReceiveBuff){
	if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_TEMP) == 0UL){
		return BT_COMMAND_GET_TEMP_ID;
	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_ACCEL) == 0UL){
		return BT_COMMAND_GET_ACCEL_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_MAG) == 0UL){
		return BT_COMMAND_GET_MAG_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_GYRO) == 0UL){
		return BT_COMMAND_GET_GYRO_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_HUM) == 0UL){
		return BT_COMMAND_GET_HUM_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_PRESS) == 0UL){
		return BT_COMMAND_GET_PRESS_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_LIGHT) == 0UL){
		return BT_COMMAND_GET_LIGHT_ID;

	} else if(strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_NOISE) == 0UL){
		return BT_COMMAND_GET_NOISE_ID;
	}
	return BT_COMMAND_GET_TEMP_ID;
}

static Retcode_T sendTempData(void){
	Retcode_T retcode = RETCODE_OK;

	uint8_t temp[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(temp, 0U, sizeof(temp));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) temp, "%f", latestSensorValue.Temp );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) temp, (uint8_t) sizeof(temp), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendAccelData(void){
	Retcode_T retcode = RETCODE_OK;

	uint8_t accel[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(accel, 0U, sizeof(accel));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) accel, "%ld %ld %ld", latestSensorValue.Accel.X, latestSensorValue.Accel.Y, latestSensorValue.Accel.Z );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) accel, (uint8_t) sizeof(accel), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendMagData(void){
	Retcode_T retcode = RETCODE_OK;

	uint8_t mag[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(mag, 0U, sizeof(mag));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) mag, "%ld %ld %ld %ld", latestSensorValue.Mag.X, latestSensorValue.Mag.Y, latestSensorValue.Mag.Z, latestSensorValue.Mag.R );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) mag, (uint8_t) sizeof(mag), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendGyroData(void){
	Retcode_T retcode = RETCODE_OK;

	uint8_t gyro[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(gyro, 0U, sizeof(gyro));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) gyro, "%ld %ld %ld", latestSensorValue.Gyro.X, latestSensorValue.Gyro.Y, latestSensorValue.Gyro.Z );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) gyro, (uint8_t) sizeof(gyro), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendHumData(void){
	Retcode_T retcode = RETCODE_OK;
	uint8_t humidity [APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(humidity, 0U, sizeof(humidity));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) humidity, "%ld", latestSensorValue.RH );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) humidity, (uint8_t) sizeof(humidity), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendPressData(void){
	Retcode_T retcode = RETCODE_OK;
	uint8_t pressure[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(pressure, 0U, sizeof(pressure));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) pressure, "%ld", latestSensorValue.Pressure );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) pressure, (uint8_t) sizeof(pressure), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendLightData(void){
	Retcode_T retcode = RETCODE_OK;
	uint8_t light[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(light, 0U, sizeof(light));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) light, "%f", latestSensorValue.Light );
	}
	if (RETCODE_OK == retcode)
	{
		if (AppControllerBleTransmitPayload && BLE_IsConnected())
		{
			retcode = BLE_SendData((uint8_t*) light, (uint8_t) sizeof(light), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
		}
	}
	return retcode;
}

static Retcode_T sendNoiseData(void){
	Retcode_T retcode = RETCODE_OK;
	uint8_t noise[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
	/* Resetting / clearing the necessary buffers / variables for re-use */
	retcode = RETCODE_OK;
	memset(noise, 0U, sizeof(noise));
	if (RETCODE_OK == retcode)
	{
		sprintf((char*) noise, "%ld", latestSensorValue.Noise );
	}
	if (RETCODE_OK == retcode)
		{
			if (AppControllerBleTransmitPayload && BLE_IsConnected())
			{
				retcode = BLE_SendData((uint8_t*) noise, (uint8_t) sizeof(noise), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
			}
		}
	return retcode;
}

static void SendLatestData(void * param1, uint32_t commandId){

    BCDS_UNUSED(param1);
    Retcode_T retcode = RETCODE_OK;

	if(commandId == BT_COMMAND_GET_TEMP_ID){
		retcode = sendTempData();
	} else if(commandId == BT_COMMAND_GET_ACCEL_ID){
		retcode = sendAccelData();
	} else if(commandId == BT_COMMAND_GET_MAG_ID) {
		retcode = sendMagData();
	} else if(commandId == BT_COMMAND_GET_GYRO_ID){
		retcode = sendGyroData();
	} else if(commandId == BT_COMMAND_GET_HUM_ID){
		retcode = sendHumData();
	} else if(commandId == BT_COMMAND_GET_PRESS_ID) {
		retcode = sendPressData();
	} else if(commandId == BT_COMMAND_GET_LIGHT_ID){
		retcode = sendLightData();
	} else if(commandId == BT_COMMAND_GET_NOISE_ID){
		retcode = sendNoiseData();
	}


	if (RETCODE_OK != retcode)
	{
		printf("Attempting to write/send failed \r\n");
		Retcode_RaiseError(retcode);
	}
}


static uint8_t isValidCommand(uint8_t* bleReceiveBuff){
	return strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_TEMP)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_ACCEL)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_MAG)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_GYRO)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_HUM)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_PRESS)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_LIGHT)
			|| strcmp((const char *) bleReceiveBuff, BT_COMMAND_GET_NOISE);
}

static void AppControllerBleDataRxCB(uint8_t *rxBuffer, uint8_t rxDataLength, void * param)
{
	BCDS_UNUSED(param);
	BCDS_UNUSED(rxDataLength);

	Retcode_T retcode = RETCODE_OK;
	uint8_t bleReceiveBuff[APP_CONTROLLER_BLE_RX_BUFFER_SIZE];
	memset(bleReceiveBuff, 0, sizeof(bleReceiveBuff));
	memcpy(bleReceiveBuff, rxBuffer, rxDataLength);
	/* validate received data */
	if (isValidCommand(bleReceiveBuff)){
		AppControllerBleTransmitPayload = true;
		retcode = CmdProcessor_Enqueue(CmdProcessorHandle, SendLatestData, NULL, getCommandId(bleReceiveBuff));
	} else {
		/* Do nothing since we are not interested in garbage */
	}

	if (RETCODE_OK != retcode)
	{
		Retcode_RaiseError(retcode);
	}
}

static void setBTData(Sensor_Value_T * sensorValue)
{
	memcpy(&sensorValue, &latestSensorValue, (sizeof sensorValue));
}

static void SetProcessor(CmdProcessor_T * processor){
	CmdProcessorHandle = processor;
}

#endif /* SOURCE_BTCONTROLLER_H_ */

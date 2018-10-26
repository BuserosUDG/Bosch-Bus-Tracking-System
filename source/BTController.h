#ifndef SOURCE_BTCONTROLLER_H_
#define SOURCE_BTCONTROLLER_H_

#include "XdkAppInfo.h"

/* additional interface header files */
#include "XDK_BLE.h"


/* constant definitions ***************************************************** */

/**
 * APP_CONTROLLER_BLE_DEVICE_NAME is the BLE device name.
 */
#define APP_CONTROLLER_BLE_DEVICE_NAME     "XDK_UDP_BLE"

/**
 * APP_CONTROLLER_BLE_START_ID is the part of the payload to be received from
 * BLE central to start streaming data via. BLE.
 */
#define APP_CONTROLLER_BLE_START_ID         "getdata"

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

static void AppControllerBleStartOrEndMsgSend(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);

    Retcode_T retcode = RETCODE_OK;

    if (param2 == APP_CONTROLLER_BLE_START_TRIGGER)
    {
        retcode = BLE_SendData(((uint8_t*) APP_CONTROLLER_BLE_START_PAYLOAD), ((uint8_t) sizeof(APP_CONTROLLER_BLE_START_PAYLOAD) - 1), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
    }
    else if (param2 == APP_CONTROLLER_BLE_END_TRIGGER)
    {
        retcode = BLE_SendData(((uint8_t*) APP_CONTROLLER_BLE_END_PAYLOAD), ((uint8_t) sizeof(APP_CONTROLLER_BLE_END_PAYLOAD) - 1), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
    }
    else
    {
        /* Do nothing */;
    }
    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
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
    if ((0UL == strcmp((const char *) bleReceiveBuff, APP_CONTROLLER_BLE_START_ID)) &&
            (false == AppControllerBleTransmitPayload))
    {
        //retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerBleStartOrEndMsgSend, NULL, APP_CONTROLLER_BLE_START_TRIGGER);
        AppControllerBleTransmitPayload = true;
    }
    else if ((0UL == strcmp((const char *) bleReceiveBuff, APP_CONTROLLER_BLE_END_ID)) &&
            (true == AppControllerBleTransmitPayload))
    {
        AppControllerBleTransmitPayload = false;
        //retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerBleStartOrEndMsgSend, NULL, APP_CONTROLLER_BLE_END_TRIGGER);
    }
    else
    {
        ;/* Do nothing since we are not interested in garbage */
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

static void sendBTData(Sensor_Value_T * sensorValue)
{

    Retcode_T retcode = RETCODE_OK;
    uint8_t accel[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t mag[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t gyro[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t humidity [APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t temp[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t pressure[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t light[APP_CONTROLLER_BLE_TX_LEN] = { 0 };
    uint8_t noise[APP_CONTROLLER_BLE_TX_LEN] = { 0 };



        /* Resetting / clearing the necessary buffers / variables for re-use */
        retcode = RETCODE_OK;
        memset(accel, 0U, sizeof(accel));
        memset(mag, 0U, sizeof(mag));
        memset(gyro, 0U, sizeof(gyro));
        memset(humidity, 0U, sizeof(humidity));
        memset(temp, 0U, sizeof(temp));
        memset(pressure, 0U, sizeof(pressure));
        memset(light, 0U, sizeof(light));
        memset(noise, 0U, sizeof(noise));

        if (RETCODE_OK == retcode)
        {
            sprintf((char*) accel, "%ld %ld %ld", sensorValue->Accel.X, sensorValue->Accel.Y, sensorValue->Accel.Z );
            sprintf((char*) mag, "%ld %ld %ld %ld", sensorValue->Mag.X, sensorValue->Mag.Y, sensorValue->Mag.Z, sensorValue->Mag.R );
            sprintf((char*) gyro, "%ld %ld %ld", sensorValue->Gyro.X, sensorValue->Gyro.Y, sensorValue->Gyro.Z );
            sprintf((char*) humidity, "%ld", sensorValue->RH );
            sprintf((char*) temp, "%f", sensorValue->Temp );
            sprintf((char*) pressure, "%ld", sensorValue->Pressure );
            sprintf((char*) light, "%f", sensorValue->Light );
            sprintf((char*) noise, "%ld", sensorValue->Noise );
        }

        if (RETCODE_OK == retcode)
        {
            if (AppControllerBleTransmitPayload && BLE_IsConnected())
            {
                retcode = BLE_SendData((uint8_t*) accel, (uint8_t) sizeof(accel), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) mag, (uint8_t) sizeof(mag), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) gyro, (uint8_t) sizeof(gyro), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) humidity, (uint8_t) sizeof(humidity), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) temp, (uint8_t) sizeof(temp), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) pressure, (uint8_t) sizeof(pressure), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) light, (uint8_t) sizeof(light), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
                retcode = BLE_SendData((uint8_t*) noise, (uint8_t) sizeof(noise), NULL, APP_CONTROLLER_BLE_SEND_TIMEOUT_IN_MS);
            }
        }
        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }
}

#endif /* SOURCE_BTCONTROLLER_H_ */

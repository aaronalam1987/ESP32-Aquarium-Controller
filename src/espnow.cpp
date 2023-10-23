#include "global.h"
#include "espNow.h"
#include "settings.h"
#include <WiFi.h>

extern Settings settings;
extern System sys;
espNow espnow;
uint8_t slaveAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
rValues relayValues{.R1Name = "", .R2Name = "", .R3Name = "", .R4Name = "", .R5Name = "", .R6Name = "", .R7Name = "", .R8Name = "", .R1_state = 0, .R2_state = 0, .R3_state = 0, .R4_state = 0, .R5_state = 0, .R6_state = 0, .R7_state = 0, .R8_state = 0};

void espNow::espNowInit()
{
    // Initialise ESP-NOW.
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("There was an error with ESP-NOW.");
        return;
    }
    // Set slaveInfo to all 0.
    memset(&slaveInfo, 0, sizeof(slaveInfo));

    // Copy slaveAddress into slaveInfo.
    memcpy(slaveInfo.peer_addr, slaveAddress, 6);
    slaveInfo.channel = 0;
    slaveInfo.encrypt = false;

    if (esp_now_add_peer(&slaveInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer.");
        return;
    }

    // Send initial data.
    sendData();
}

void espNow::sendData()
{
    // Assign struct values used for ESP-NOW statusArray values set by equipmentControl.cpp.
    // Set relay names.
    relayValues.R1Name = settings.deviceOneName;
    relayValues.R2Name = settings.deviceTwoName;
    relayValues.R3Name = settings.deviceThreeName;
    relayValues.R4Name = settings.deviceFourName;
    relayValues.R5Name = settings.deviceFiveName;
    relayValues.R6Name = settings.deviceSixName;
    relayValues.R7Name = settings.deviceSevenName;
    relayValues.R8Name = settings.deviceEightName;

    // Set relay state (0/1).
    relayValues.R1_state = sys.getStatusArray(0, 1);
    relayValues.R2_state = sys.getStatusArray(1, 1);
    relayValues.R3_state = sys.getStatusArray(2, 1);
    relayValues.R4_state = sys.getStatusArray(3, 1);
    relayValues.R5_state = sys.getStatusArray(4, 1);
    relayValues.R6_state = sys.getStatusArray(5, 1);
    relayValues.R7_state = sys.getStatusArray(6, 1);
    relayValues.R8_state = sys.getStatusArray(7, 1);

    esp_err_t result = esp_now_send(slaveAddress, (uint8_t *)&relayValues, sizeof(relayValues));
    if (result == ESP_OK)
    {
        Serial.println("ESP-NOW Data Sent.");
    }
    else
    {
        Serial.println("ESP-NOW Data Failed.");
    }
}
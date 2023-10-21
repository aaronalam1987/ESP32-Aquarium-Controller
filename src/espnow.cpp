#include "global.h"
#include "espnow.h"

espNow espnow;
esp_now_peer_info_t slaveInfo;

extern System sys;
uint8_t slaveAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

rValues relayValues{.R1 = 0, .R2 = 0, .R3 = 0, .R4 = 0};

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

    // Send intial values.
    relayValues.R1 = 12;
    esp_err_t result = esp_now_send(slaveAddress, (uint8_t *)&relayValues, sizeof(relayValues));
}

void espNow::sendData()
{
    // Assign struct values used for ESP-NOW statusArray values set by equipmentControl.cpp.
    relayValues.R1 = sys.getStatusArray(0, 1);
    relayValues.R2 = sys.getStatusArray(1, 1);
    relayValues.R3 = sys.getStatusArray(2, 1);
    relayValues.R4 = sys.getStatusArray(3, 1);

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
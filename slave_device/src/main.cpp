#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);

String status = "Waiting...";

int channel = 6;

struct rValues
{
  int R1;
  int R2;
  int R3;
  int R4;
};

rValues relayValues;

String equipmentStatus(int value)
{
  return (value == 1) ? "On" : "Off";
}

void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Set relayValues struct to all 0.
  memset(&relayValues, 0, sizeof(relayValues));

  // Copy "incomingData" to relayValues struct.
  memcpy(&relayValues, incomingData, sizeof(relayValues));

  // Set connected relay to specified value.
  digitalWrite(13, relayValues.R1);

  Serial.println(relayValues.R1);
  if (relayValues.R1 == 12)
  {
    status = "Paired";
  }
}

void setup()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Failed to init screen");
    for (;;)
      ;
  }

  // Set display up.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Serial.begin(9600);
  // Define our connected relays.
  pinMode(13, OUTPUT);

  // Set WiFi.
  WiFi.mode(WIFI_STA);

  // Initalise ESP-NOW.
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("There was an error with ESP-NOW.");
    return;
  }

  // As the master device connects to WiFi, it's channel is set to match the wifi host.
  // Set our channel to match.
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  // Define function used upon data received.
  esp_now_register_recv_cb(dataReceived);
}

void loop()

{
  // Clear display and draw updated values.
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Channel:" + String(channel));
  // display.setCursor(70, 0);
  // display.print(status);
  display.setCursor(0, 8);
  display.print("One:" + equipmentStatus(relayValues.R1));
  display.setCursor(64, 8);
  display.print("Two:" + equipmentStatus(relayValues.R2));
  display.setCursor(0, 16);
  display.print("Three:" + equipmentStatus(relayValues.R2));
  display.setCursor(64, 16);
  display.print("Four:" + equipmentStatus(relayValues.R2));
  display.setCursor(0, 24);
  display.print("Five:" + equipmentStatus(relayValues.R1));
  display.setCursor(64, 24);
  display.print("Six:" + equipmentStatus(relayValues.R2));
  display.setCursor(0, 32);
  display.print("Seven:" + equipmentStatus(relayValues.R2));
  display.setCursor(64, 32);
  display.print("Eight:" + equipmentStatus(relayValues.R2));
  display.display();
  Serial.println(status);
  Serial.println(relayValues.R1);
  Serial.println(relayValues.R2);
  Serial.println(relayValues.R3);
  Serial.println(relayValues.R4);
}
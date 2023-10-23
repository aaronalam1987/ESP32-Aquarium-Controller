#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

int channel = 6;
int page = 0;
unsigned long previousMillis = 0;

struct rValues
{
  String R1Name;
  String R2Name;
  String R3Name;
  String R4Name;
  String R5Name;
  String R6Name;
  String R7Name;
  String R8Name;
  int R1_state;
  int R2_state;
  int R3_state;
  int R4_state;
  int R5_state;
  int R6_state;
  int R7_state;
  int R8_state;
};

rValues relayValues;

String equipmentStatus(int value)
{
  return (value == 1) ? "On" : "Off";
}

void setPage()
{
  if (page == 0)
  {
    // Set page 0 values which display the data for relays 1-4.
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Channel:" + String(channel));
    display.setCursor(0, 8);
    display.print("Mac:" + WiFi.macAddress());
    display.setCursor(0, 16);
    display.print(relayValues.R1Name + ": " + equipmentStatus(relayValues.R1_state));
    display.setCursor(0, 24);
    display.print(relayValues.R2Name + ": " + equipmentStatus(relayValues.R2_state));
    display.setCursor(0, 32);
    display.print(relayValues.R3Name + ": " + equipmentStatus(relayValues.R3_state));
    display.setCursor(0, 40);
    display.print(relayValues.R4Name + ": " + equipmentStatus(relayValues.R4_state));
    display.display();
  }
  else
  {
    // Set page 1 values which display the data for relays 5-8.
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Channel:" + String(channel));
    display.setCursor(0, 8);
    display.print("Mac:" + WiFi.macAddress());
    display.setCursor(0, 16);
    display.print(relayValues.R5Name + ": " + equipmentStatus(relayValues.R5_state));
    display.setCursor(0, 24);
    display.print(relayValues.R6Name + ": " + equipmentStatus(relayValues.R6_state));
    display.setCursor(0, 32);
    display.print(relayValues.R7Name + ": " + equipmentStatus(relayValues.R7_state));
    display.setCursor(0, 40);
    display.print(relayValues.R8Name + ": " + equipmentStatus(relayValues.R8_state));
    display.display();
  }
}

void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Set relayValues struct to all 0.
  memset(&relayValues, 0, sizeof(relayValues));

  // Copy "incomingData" to relayValues struct.
  memcpy(&relayValues, incomingData, sizeof(relayValues));

  // Set connected relays to specified value.
  digitalWrite(13, relayValues.R1_state);

  // Update display with new data.
  setPage();
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
  setPage();

  // Used for debug only.
  Serial.begin(9600);

  // Define our connected relays.
  pinMode(13, OUTPUT);

  // Set WiFi.
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleep(WIFI_PS_NONE);

  // Initalise ESP-NOW.
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("There was an error with ESP-NOW.");
    return;
  }

  // As the master device connects to WiFi, it's channel is set to match the wifi host.
  // Set our channel to match using the configurable channel defined by channel int.
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  // Define function used upon data received.
  esp_now_register_recv_cb(dataReceived);
}

void loop()
{
  // Update page number every 3 seconds.
  // Each page displays the status of 4 relays.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 3000)
  {
    previousMillis = currentMillis;
    page = page == 0 ? 1 : 0;
    setPage();
  }
}
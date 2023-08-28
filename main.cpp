#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "global.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
// Task handler for temperature probe monitoring and WiFi connection.

TaskHandle_t doWifi;
TaskHandle_t monitorTemp;

// Initialise global ints used to keep track of menus and to assign buttons.
int currentMenu = 0;
int previousMenu = 0;
int menuChanged = 0;
int menuSelect = 0;
int buttonUp = 0;
int buttonDown = 0;
int buttonBack = 0;
int buttonOK = 0;

double tempLog[2][24] = {{0}};

// Bools used to identify if an alert is present, if waterchange/feed function is enabled, if eeprom should be written and to identify selectable menus.
bool alert = false;
bool waterChange = false;
bool eepromWrite = false;
bool selectableMenu;

String selectIndicator = "";

// Settings struct.
settings Settings;

// Assigned equipment using connectedEquipment struct.
connectedEquipment
    deviceOne,
    deviceTwo, deviceThree, deviceFour, deviceFive, deviceSix, deviceSeven, deviceEight;

String currentStatus(int status)
{
  // Converts the status int into a readable string, to be used in menus.
  // 1 represents the relay being "off" as connected devices are connected via "normally closed" to be safe in the event of some sort of system failure.
  String currStatus = "";
  if (status == 0)
  {
    currStatus = " - Off";
  }
  else
  {
    currStatus = " - On ";
  }
  return currStatus;
}

String menuArray[11][4] = {
    // Init Menu.
    {"Temp:Waiting...", "IP: No Connection", "Alerts:None", "Time: Not Set"},
    // Options Page One.
    {"Equipment Control", "Temperature Control", "Start Water Change", "Set Light Timer"},
    // Options One Elements start.
    {"", "", "", ""},
    {"Target Temp: " + String(Settings.targetTemp), "Variance: " + String(Settings.tempVariant), "Alert: " + String(Settings.tempAlert), "               Save"},
    {"Ok to enable/disable", "", "", ""},
    {"Light On : " + String(Settings.lightOn), "Light Off: " + String(Settings.lightOff), "Cycle with OK", "Select to set"},
    // Options One Elements End.
    // Options Page Two.
    {"Equipment Settings", "WiFi Settings", "Web Server", ""},
    // Options Two Elements Start.
    {"Equipment Settings", "Equipment Settings", "Equipment Settings", "Equipment Settings"},
    {"", "", "", ""},
    {"", "", "", ""},
    {"Web Server: ", "Enabled", "", ""}
    // Options Two Elements End.
};

void initMenu()
{
  // Menu used to display current temperature, heating/cooling mode, local IP and alerts (if any).
  selectableMenu = false;
  // OK press sets current menu to options menu.
  if (buttonOK == LOW)
  {
    currentMenu = 1;
  }
}

void equipmentControl()
{
  // This function alternates specified relay on or off.
  selectableMenu = true;
  if (buttonOK == LOW)
  {
    switch (menuSelect)
    {
    case 0:
      deviceOne.status == 0 ? deviceOne.status = 1 : deviceOne.status = 0;
      menuArray[2][0] = deviceOne.name + currentStatus(deviceOne.status);
      digitalWrite(deviceOne.pin, deviceOne.status);
      break;
    case 1:
      deviceTwo.status == 0 ? deviceTwo.status = 1 : deviceTwo.status = 0;
      menuArray[2][1] = deviceTwo.name + currentStatus(deviceTwo.status);
      digitalWrite(deviceTwo.pin, deviceTwo.status);
      break;
    case 2:
      deviceThree.status == 0 ? deviceThree.status = 1 : deviceThree.status = 0;
      menuArray[2][2] = deviceThree.name + currentStatus(deviceThree.status);
      digitalWrite(deviceThree.pin, deviceThree.status);
      break;
    case 3:
      deviceFour.status == 0 ? deviceFour.status = 1 : deviceFour.status = 0;
      menuArray[2][3] = deviceFour.name + currentStatus(deviceFour.status);
      digitalWrite(deviceFour.pin, deviceFour.status);
      break;
    }
  }
}

void startWaterChange()
{
  // Waterchange is designed to disable all but specified devices and this function simply updates the array with the status and sets the bool to true or false.
  selectableMenu = false;
  if (waterChange)
  {
    menuArray[4][2] = "Status: Enabled ";
  }
  else
  {
    menuArray[4][2] = "Status: Disabled";
  }
  if (buttonOK == LOW)
  {
    waterChange == true ? waterChange = false : waterChange = true;
  }
}

void setLightTimer()
{
  // Todo
  selectableMenu = false;
}

void clearLine(int line)
{
  lcd.setCursor(0, line);
  lcd.print("                    ");
}

void setup()
{
  // Read config file and assign values to settings struct.
  readEeprom();

  // Assign values to device struct objects.
  deviceOne = {Settings.deviceOneName, 1, 27},
  deviceTwo = {Settings.deviceTwoName, 1, 26},
  deviceThree = {Settings.deviceThreeName, 1, 25},
  deviceFour = {Settings.deviceFourName, 1, 33},
  deviceFive = {Settings.deviceFiveName, 1, 32},
  deviceSix = {Settings.deviceSixName, 1, 17},
  deviceSeven = {Settings.deviceSevenName, 1, 18},
  deviceEight = {Settings.deviceEightName, 1, 19};

  // Amend array with device names and status.
  menuArray[2][0] = deviceOne.name + currentStatus(deviceOne.status);
  menuArray[2][1] = deviceTwo.name + currentStatus(deviceTwo.status);
  menuArray[2][2] = deviceThree.name + currentStatus(deviceThree.status);
  menuArray[2][3] = deviceFour.name + currentStatus(deviceFour.status);

  // Assign "monitorTemp" function to seperate task to remove UI delay from probing.
  xTaskCreate(
      monitorTempFunction,
      "monitorTemp",
      10000,
      NULL,
      0,
      &monitorTemp);
  // Assign WiFi function to seperate task, preventing UI delay.
  // Task is deleted upon successful WiFi connection.
  xTaskCreate(
      doWiFi,
      "do Wifi",
      10000,
      NULL,
      0,
      &doWifi);
  // Only used for debugging.
  Serial.begin(9600);
  // Initilise LCD.
  lcd.init();
  lcd.clear();
  lcd.backlight();
  // Assign the up, down, back and OK buttons.
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(36, INPUT);
  pinMode(39, INPUT);
  // Assign relay/equipment pins based on struct values.
  pinMode(deviceOne.pin, OUTPUT);
  pinMode(deviceTwo.pin, OUTPUT);
  pinMode(deviceThree.pin, OUTPUT);
  pinMode(deviceFour.pin, OUTPUT);
  pinMode(deviceFive.pin, OUTPUT);
  pinMode(deviceSix.pin, OUTPUT);
  pinMode(deviceSeven.pin, OUTPUT);
  pinMode(deviceEight.pin, OUTPUT);

  // Set all relays to initial state (relay is off, device connected via N/C);
  digitalWrite(deviceOne.pin, 1);
  digitalWrite(deviceTwo.pin, 1);
  digitalWrite(deviceThree.pin, 1);
  digitalWrite(deviceFour.pin, 1);
  digitalWrite(deviceFive.pin, 1);
  digitalWrite(deviceSix.pin, 1);
  digitalWrite(deviceSeven.pin, 1);
  digitalWrite(deviceEight.pin, 1);
}

void loop()
{
  Serial.println(currentTemp);
  buttonUp = digitalRead(34);
  buttonDown = digitalRead(35);
  buttonBack = digitalRead(36);
  buttonOK = digitalRead(39);

  writeEeprom();

  if (alert)
  {
    tone(12, 500, 500);
    alert = false;
  }

  switch (currentMenu)
  {
  case 0:
    initMenu();
    break;
  case 1:
    optionsMenu();
    break;
  case 2:
    equipmentControl();
    break;
  case 3:
    tempControl();
    break;
  case 4:
    startWaterChange();
    break;
  case 5:
    setLightTimer();
    break;
  case 6:
    optionsMenuMore();
    break;
  case 8:
    setWifi();
    break;
  }
  timeMonitor();
  inputMonitor();
  drawMenu();
  delay(100);
}

#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "global.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Task handler for temperature probe monitoring and WiFi connection.

TaskHandle_t doWifi;
TaskHandle_t monitorTemp;
// Wifi Credentials.

// Ints used to keep track of menus and to assign buttons.
int currentMenu = 0;
int previousMenu = 0;
int menuChanged = 0;
int menuSelect = 0;
int buttonUp = 0;
int buttonDown = 0;
int buttonBack = 0;
int buttonOK = 0;

// Bools used to identify if an alert is present, if waterchange/feed function is enabled, if eeprom should be written and to identify selectable menus.
bool alert = false;
bool waterChange = false;
bool eepromWrite = false;
bool selectableMenu;

String selectIndicator = "";

// Assigned equipment using connectedEquipment struct.
connectedEquipment equipOne = {"Lights", 1, 27};
connectedEquipment equipTwo = {"Powerhead", 1, 26};
connectedEquipment equipThree = {"Heater", 1, 25};
connectedEquipment equipFour = {"Return", 1, 33};

// Settings struct.
settings Settings;

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
    {"Temp:Waiting...", "IP: No Connection", "Alerts:None", "      Ok for options"},
    // Options Page One.
    {"Equipment Control", "Temperature Control", "Start Water Change", "Set Light Timer"},
    // Options One Elements start.
    {equipOne.name + currentStatus(equipOne.status), equipTwo.name + currentStatus(equipTwo.status), equipThree.name + currentStatus(equipThree.status), equipFour.name + currentStatus(equipFour.status)},
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

void setEquipment()
{
  // Function to set the relay for each connected peice of equipment.
  // If waterChange is enabled it allows, in this configuration, for all equipment to be turned off except for lights.
  if (waterChange)
  {
    digitalWrite(equipOne.pin, 1);
    digitalWrite(equipTwo.pin, 0);
    digitalWrite(equipThree.pin, 0);
    digitalWrite(equipFour.pin, 0);
  }
  else
  {
    digitalWrite(equipOne.pin, equipOne.status);
    digitalWrite(equipTwo.pin, equipTwo.status);
    digitalWrite(equipThree.pin, equipThree.status);
    digitalWrite(equipFour.pin, equipFour.status);
  }
}

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
      equipOne.status == 0 ? equipOne.status = 1 : equipOne.status = 0;
      menuArray[2][0] = equipOne.name + currentStatus(equipOne.status);
      break;
    case 1:
      equipTwo.status == 0 ? equipTwo.status = 1 : equipTwo.status = 0;
      menuArray[2][1] = equipTwo.name + currentStatus(equipTwo.status);
      break;
    case 2:
      equipThree.status == 0 ? equipThree.status = 1 : equipThree.status = 0;
      menuArray[2][2] = equipThree.name + currentStatus(equipThree.status);
      break;
    case 3:
      equipFour.status == 0 ? equipFour.status = 1 : equipFour.status = 0;
      menuArray[2][3] = equipFour.name + currentStatus(equipFour.status);
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
  // Assign relay/equipment pins based on struct values, more to add.
  pinMode(equipOne.pin, OUTPUT);
  pinMode(equipTwo.pin, OUTPUT);
  pinMode(equipThree.pin, OUTPUT);
  pinMode(equipFour.pin, OUTPUT);
}

void loop()
{
  buttonUp = digitalRead(34);
  buttonDown = digitalRead(35);
  buttonBack = digitalRead(36);
  buttonOK = digitalRead(39);
  webServer();
  writeEeprom();
  setEquipment();

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
  inputMonitor();
  drawMenu();
  delay(100);
}

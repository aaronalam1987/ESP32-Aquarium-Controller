#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <map>
#include "global.h"

Menus menu;
Settings settings;
extern TempMonitor tempMonitor;
extern WIFI wifi;

int statusArray[8][2]{{1}};
// Task handler for temperature probe monitoring and WiFi connection.
TaskHandle_t doWifi;
TaskHandle_t monitorTemp;

// Initialise global ints used to keep track of menus and to assign buttons.
int currentMenu{0}, previousMenu{0}, menuChanged{0}, menuSelect{0}, buttonUp{0}, buttonDown{0}, buttonBack{0}, buttonOK{0};

// tempLog is used to store hourly temperature values, used for both 24hr and 48hr storage.
double tempLog[2][24] = {{0}};

// Bools used to identify if an alert is present, if waterchange/feed function is enabled, if eeprom should be written and to identify selectable menus.
bool alert = false, waterChange = false, eepromWrite = false;

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

void initMenu()
{
  // Menu used to display current temperature, heating/cooling mode, local IP and alerts (if any).
  menu.setMenu(0, 0, "Temp:" + String(tempMonitor.getCurrentTemp()) + " Mode:" + tempMonitor.getCurrentMode());
  menu.setMenu(0, 1, wifi.getCurrentStatus());
  menu.setMenu(0, 2, "Alerts: None        ");
  menu.setMenu(0, 3, "Time: Not Set       ");
  menu.menuSelectable(false);

  // OK press sets current menu to options menu.
  if (buttonOK == LOW)
  {
    menu.setCurrentMenu(menuOptions);
  }
}

void equipmentControl()
{
  // Map the currently selected menu line to the associated GPIO.
  std::map<int, int> gpioMap;
  gpioMap[menuLineOne] = 27;
  gpioMap[menuLineTwo] = 26;
  gpioMap[menuLineThree] = 25;
  gpioMap[menuLineFour] = 33;

  // statusArray allows us to associate a value to each gpio pin allowing us to know if the device is off (0) or on (1)
  statusArray[0][0] = gpioMap[menuLineOne];
  statusArray[1][0] = gpioMap[menuLineTwo];
  statusArray[2][0] = gpioMap[menuLineThree];
  statusArray[3][0] = gpioMap[menuLineFour];

  // Clear and build menu.
  menu.clearMenu();
  menu.setMenu(0, 0, deviceOne.name + currentStatus(statusArray[0][1]));
  menu.setMenu(0, 1, deviceTwo.name + currentStatus(statusArray[1][1]));
  menu.setMenu(0, 2, deviceThree.name + currentStatus(statusArray[2][1]));
  menu.setMenu(0, 3, deviceFour.name + currentStatus(statusArray[3][1]));
  menu.menuSelectable(true);

  if (buttonOK == LOW)
  {
    for (int i = 0; i < 8; i++)
    {
      // This allows us to alternate specified relay on or off.
      if (statusArray[i][0] == gpioMap[menu.getMenuSelect()])
      {
        if (statusArray[i][1] == 0)
        {
          statusArray[i][1] = 1;
        }
        else
        {
          statusArray[i][1] = 0;
        }
        digitalWrite(gpioMap[menu.getMenuSelect()], statusArray[i][1]);
      }
    }
  }
}

void setLightTimer()
{
  // Todo
  menu.menuSelectable(false);
}

void setup()
{

  // Read config file and assign values to settings struct.
  settings.loadSettings();
  // Assign values to device struct objects.
  deviceOne = {settings.deviceOneName, 1, 27},
  deviceTwo = {settings.deviceTwoName, 1, 26},
  deviceThree = {settings.deviceThreeName, 1, 25},
  deviceFour = {settings.deviceFourName, 1, 33},
  deviceFive = {settings.deviceFiveName, 1, 32},
  deviceSix = {settings.deviceSixName, 1, 17},
  deviceSeven = {settings.deviceSevenName, 1, 18},
  deviceEight = {settings.deviceEightName, 1, 19};

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
  menu.lcdInit();
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
  digitalWrite(deviceOne.pin, gpioON);
  digitalWrite(deviceTwo.pin, gpioON);
  digitalWrite(deviceThree.pin, gpioON);
  digitalWrite(deviceFour.pin, gpioON);
  digitalWrite(deviceFive.pin, gpioON);
  digitalWrite(deviceSix.pin, gpioON);
  digitalWrite(deviceSeven.pin, gpioON);
  digitalWrite(deviceEight.pin, gpioON);

  // Initialise Async webserver.
  webServer();
}

void loop()
{
  buttonUp = digitalRead(34);
  buttonDown = digitalRead(35);
  buttonBack = digitalRead(36);
  buttonOK = digitalRead(39);

  // Array of function pointers.
  // Use a map to link the currentMenu int to the associated function pages.
  std::map<int, std::function<void()>> menuActions;
  menuActions[0] = initMenu;
  menuActions[1] = optionsMenu;
  menuActions[2] = equipmentControl;
  menuActions[3] = tempControl;
  menuActions[4] = lightControl;
  menuActions[5] = settingsConfig;

  // Use currentMenu to navigate to associated function page.
  menuActions[menu.getCurrentMenu()]();

  timeMonitor();
  inputMonitor();
  menu.drawMenu();
  delay(100);
}

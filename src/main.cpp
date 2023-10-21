#include <map>
#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "wifiFunctions.h"
#include "menuControl.h"
#include "espnow.h"

extern Menus menu;
Settings settings;
extern TempMonitor tempMonitor;
extern WIFI wifi;
System sys;

extern espNow espnow;

// Task handler for temperature probe monitoring and WiFi connection.
TaskHandle_t doWifi;
TaskHandle_t monitorTemp;

void initMenu()
{
  if (sys.getEepromWrite())
  {
    settings.saveSettings();
    sys.setEepromWrite(false);
  }
  menu.clearMenu();
  // Menu used to display current temperature, heating/cooling mode, local IP and alerts (if any).
  menu.setMenu(0, menuLineOne, "Temp:" + String(tempMonitor.getCurrentTemp()) + " Mode:" + tempMonitor.getCurrentMode());
  menu.setMenu(0, menuLineTwo, wifi.getCurrentStatus());
  menu.setMenu(0, menuLineThree, "Alerts: " + sys.getAlert());
  menu.setMenu(0, menuLineFour, "Time: " + sys.getCurrentTime());
  menu.menuSelectable(false);

  // OK press sets current menu to options menu.
  if (sys.buttonOK == LOW)
  {
    menu.setCurrentMenu(menuOptions);
  }
}

void setup()
{

  // Read config file and assign values to settings class.
  settings.loadSettings();

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

  // Assign relay/equipment pins.
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  // Set all relays to initial state (relay is off, device connected via N/C);
  digitalWrite(27, gpioON);
  digitalWrite(26, gpioON);
  digitalWrite(25, gpioON);
  digitalWrite(33, gpioON);
  digitalWrite(32, gpioON);
  digitalWrite(17, gpioON);
  digitalWrite(18, gpioON);
  digitalWrite(19, gpioON);

  // Initialise Async webserver.
  wifi.webServer();

  espnow.espNowInit();
}

void loop()
{
  sys.buttonUp = digitalRead(34);
  sys.buttonDown = digitalRead(35);
  sys.buttonBack = digitalRead(36);
  sys.buttonOK = digitalRead(39);

  // Array of function pointers.
  // Use a map to link the currentMenu int to the associated function pages.
  std::map<int, std::function<void()>> menuActions;
  menuActions[0] = initMenu;
  menuActions[1] = optionsMenu;
  menuActions[2] = equipmentControl;
  menuActions[3] = tempControl;
  menuActions[4] = lightControl;
  menuActions[5] = std::bind(&WIFI::settingsConfig, &wifi);

  // Use currentMenu to navigate to associated function page.
  menuActions[menu.getCurrentMenu()]();

  timeMonitor();
  inputMonitor();

  menu.drawMenu();
  delay(100);
  Serial.println(WiFi.channel());
  Serial.println(sys.getStatusArray(0, 1));
}

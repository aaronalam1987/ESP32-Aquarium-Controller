#include <map>
#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "wifiFunctions.h"
#include "menuControl.h"

extern Menus menu;
Settings settings;
extern TempMonitor tempMonitor;
extern WIFI wifi;
System sys;

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

  // If you do NOT need local relays (EG: you only use slave device), you do NOT need to specify this.
  // Assign relay/equipment pins.
  // Relays should be connected as such:
  // GPIO 27 - RELAY 1 (HEATING)
  // GPIO 26 - RELAY 2 (COOLING)
  // GPIO 25 - RELAY 3 (LIGHTING)
  // GPIO 33 - RELAY 4 (ATO)
  // GPIO 32 - RELAY 5 (N/A)
  // GPIO 17 - RELAY 6 (N/A)
  // GPIO 18 - RELAY 7 (N/A)
  // GPIO 19 - RELAY 8 (N/A)
  // Relay order is important as relay 1, 2, 3 and 4 must be specific to enable automated heating/cooling, lighting and ATO.
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  // If you do NOT need local relays (EG: you only use slave device), you do NOT need to specify this.
  // Set all relays to initial state (relay is off, device connected via N/C);
  digitalWrite(27, ON);
  digitalWrite(26, ON);
  digitalWrite(25, ON);
  digitalWrite(33, ON);
  digitalWrite(32, ON);
  digitalWrite(17, ON);
  digitalWrite(18, ON);
  digitalWrite(19, ON);

  // Initialise Async webserver.
  wifi.webServer();
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
  Serial.println(settings.enableTempCon);
}

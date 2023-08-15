#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

struct connectedEquipment
{
public:
    String name;
    int status;
    int pin;
};

struct settings
{
public:
    double targetTemp;
    double tempVariant;
    double tempAlert;
    double lightOn;
    double lightOff;
    String wifiSsid;
    String wifiPassword;
};

extern String menuArray[11][4];

extern int currentMenu;
extern int previousMenu;
extern int menuChanged;
extern int menuSelect;
extern int buttonUp;
extern int buttonDown;
extern int buttonBack;
extern int buttonOK;

extern String selectIndicator;

extern bool alert;
extern bool waterChange;
extern bool eepromWrite;
extern bool selectableMenu;
extern LiquidCrystal_I2C lcd;

void inputMonitor();
void tempControl();
void clearLine(int line);
void writeEeprom();
void readEeprom();
void drawMenu();
void monitorTempFunction(void *parameter);
void doWiFi(void *parameter);
void webServer();
void optionsMenu();
void optionsMenuMore();
void menuChange();
void scanSSID();
void setWifi();

#endif
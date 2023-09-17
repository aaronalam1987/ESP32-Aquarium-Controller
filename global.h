#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Preferences.h>

constexpr int menuInit{0}, menuOptions{1}, equipmentMenu{2}, tempControlMenu{3}, lightControlMenu{4}, settingsConfigMenu{5};
constexpr int menuLineOne{0}, menuLineTwo{1}, menuLineThree{2}, menuLineFour{3};
constexpr int gpioOFF{0}, gpioON{1};

class LCD
{
private:
    LiquidCrystal_I2C lcd;

public:
    LCD() : lcd(0x27, 20, 4)
    {
    }

    void initialise()
    {
        lcd.init();
        lcd.clear();
        lcd.backlight();
    }

    void print(String text, int col, int row)
    {
        lcd.setCursor(col, row);
        lcd.print(text);
    }

    void clearLine(int line)
    {
        lcd.setCursor(0, line);
        lcd.print("                    ");
    }

    void clear()
    {
        lcd.clear();
    }
};

class TempMonitor
{
private:
    OneWire *oneWire;
    DallasTemperature *tempSensor;
    double currentTemp;
    double temperatureLog[2][24];
    String currentMode;

public:
    TempMonitor(OneWire *wire, DallasTemperature *sensor) : oneWire(wire), tempSensor(sensor), temperatureLog{{0}}, currentTemp{0}, currentMode("Wait") {}
    void initialise()
    {
        tempSensor->begin();
    }
    void checkCurrentTemp()
    {
        tempSensor->requestTemperatures();
        currentTemp = tempSensor->getTempCByIndex(0);
    }
    double getCurrentTemp()
    {
        return currentTemp;
    }
    void setTempLog(int col, int row, double entry)
    {
        temperatureLog[col][row] = entry;
    }
    double getTempLog(int col, int row)
    {
        return temperatureLog[col][row];
    }
    String getCurrentMode()
    {
        return currentMode;
    }
    void setMode(String mode)
    {
        currentMode = mode;
    }
};

class Menus
{

private:
    String menuArray[1][4];
    int currentMenu;
    int menuChanged;
    int menuSelect;
    LCD lcd;
    bool selectableMenu;

public:
    Menus() : menuArray{""}, currentMenu{0}, menuChanged{0}, selectableMenu{false} {}
    int getCurrentMenu()
    {
        return currentMenu;
    }
    int getMenuChanged()
    {
        return menuChanged;
    }
    void setMenuChanged()
    {
        menuChanged = currentMenu;
    }
    int getMenuSelect()
    {
        return menuSelect;
    }
    void setMenuSelect(int setMenuSelect)
    {
        menuSelect = setMenuSelect;
    }
    void setCurrentMenu(int currMenu)
    {
        currentMenu = currMenu;
    }
    void setMenu(int col, int row, String menuText)
    {
        menuArray[col][row] = menuText;
    }
    void menuSelectable(bool selectable)
    {
        selectableMenu = selectable;
    }
    void lcdInit()
    {
        lcd.initialise();
    }
    void clearMenu()
    {
        if (menuChanged != currentMenu)
        {
            menuChanged = currentMenu;
            lcd.clear();
        }
    }
    void drawMenu()
    {
        String selected = "";
        for (int i = 0; i <= 3; i++)
        {
            if (menuSelect == i && selectableMenu)
            {
                selected = ">";
            }

            lcd.print(selected + menuArray[0][i], 0, i);

            selected = "";
        }
    }
};

struct connectedEquipment
{
public:
    String name;
    int status;
    int pin;
};

/*struct settings
{
public:
    double targetTemp;
    double tempVariant;
    double tempAlert;
    double lightOn;
    double lightOff;
    int lightGPIO;
    int heatingGPIO;
    int coolingGPIO;
    int atoGPIO;
    String wifiSsid;
    String wifiPassword;
    String deviceOneName;
    String deviceTwoName;
    String deviceThreeName;
    String deviceFourName;
    String deviceFiveName;
    String deviceSixName;
    String deviceSevenName;
    String deviceEightName;
};*/

class Settings
{
private:
    Preferences config;

public:
    double targetTemp;
    double tempVariant;
    double tempAlert;
    double lightOn;
    double lightOff;
    int lightGPIO;
    int heatingGPIO;
    int coolingGPIO;
    int atoGPIO;
    String wifiSsid;
    String wifiPassword;
    String deviceOneName;
    String deviceTwoName;
    String deviceThreeName;
    String deviceFourName;
    String deviceFiveName;
    String deviceSixName;
    String deviceSevenName;
    String deviceEightName;

    Settings() : targetTemp{0}, tempVariant{0}, tempAlert{0}, lightOn{0}, lightOff{0}, lightGPIO{0}, heatingGPIO{0}, coolingGPIO{0}, atoGPIO{0}, wifiSsid{""}, wifiPassword{""}, deviceOneName{""}, deviceTwoName{""}, deviceThreeName{""}, deviceFourName{""}, deviceFiveName{""}, deviceSixName{""}, deviceSevenName{""}, deviceEightName{""} {};

    void loadSettings()
    {
        config.begin("config", false);
        targetTemp = config.getDouble("targetTemp", 0);
        tempVariant = config.getDouble("tempVariant", 0);
        tempAlert = config.getDouble("tempAlert", 0);
        lightOn = config.getDouble("lightOn", 0);
        lightOff = config.getDouble("lightOff", 0);
        wifiSsid = config.getString("wifiSsid", "");
        wifiPassword = config.getString("wifiPassword", "");
        deviceOneName = config.getString("deviceOneName", "");
        deviceTwoName = config.getString("deviceTwoName", "");
        deviceThreeName = config.getString("deviceThreeName", "");
        deviceFourName = config.getString("deviceFourName", "");
        deviceFiveName = config.getString("deviceFiveName", "");
        deviceSixName = config.getString("deviceSixName", "");
        deviceSevenName = config.getString("deviceSevenName", "");
        deviceEightName = config.getString("deviceEightName", "");
        lightGPIO = config.getInt("lightGPIO", 0);
        heatingGPIO = config.getInt("heatingGPIO", 0);
        coolingGPIO = config.getInt("coolingGPIO", 0);
        atoGPIO = config.getInt("atoGPIO", 0);
    }
    void saveSettings()
    {
        config.putDouble("targetTemp", targetTemp);
        config.putDouble("tempVariant", tempVariant);
        config.putDouble("tempAlert", tempAlert);
        config.putDouble("lightOn", lightOn);
        config.putDouble("lightOff", lightOff);
        config.putString("wifiSsid", wifiSsid);
        config.putString("wifiPassword", wifiPassword);
        config.putString("deviceOneName", deviceOneName);
        config.putString("deviceTwoName", deviceTwoName);
        config.putString("deviceThreeName", deviceThreeName);
        config.putString("deviceFourName", deviceFourName);
        config.putString("deviceFiveName", deviceFiveName);
        config.putString("deviceSixName", deviceSixName);
        config.putString("deviceSevenName", deviceSevenName);
        config.putString("deviceEightName", deviceEightName);
        config.putInt("lightGPIO", lightGPIO);
        config.putInt("heatingGPIO", heatingGPIO);
        config.putInt("coolingGPIO", coolingGPIO);
        config.putInt("atoGPIO", atoGPIO);
    }
};

class WIFI
{
private:
    String wifiStatus;

public:
    WIFI() : wifiStatus{"Not Set             "} {};

    void setStatus(String status)
    {
        wifiStatus = status;
    }

    String getCurrentStatus()
    {
        return wifiStatus;
    }
};

extern int buttonUp;
extern int buttonDown;
extern int buttonBack;
extern int buttonOK;
extern double tempLog[2][24];

extern bool alert;

extern bool eepromWrite;

extern struct tm timeinfo;

void inputMonitor();
void tempControl();
void clearLine(int line);

void monitorTempFunction(void *parameter);
void doWiFi(void *parameter);
void webServer();
void optionsMenu();
void menuChange();

void settingsConfig();
void setRTC();
void timeMonitor();
void lightControl();
void webServer();

#endif
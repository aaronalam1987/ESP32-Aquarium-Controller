#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>

constexpr int menuInit{0}, menuOptions{1}, equipmentMenu{2}, tempControlMenu{3}, lightControlMenu{4}, settingsConfigMenu{5};
constexpr int menuLineOne{0}, menuLineTwo{1}, menuLineThree{2}, menuLineFour{3};
constexpr int gpioOFF{0}, gpioON{1};

class System
{
private:
    String currentTime;
    String alert;
    bool eepromWrite;
    int statusArray[8][2];

public:
    int buttonUp, buttonDown, buttonBack, buttonOK;
    System() : currentTime{"Not Set"}, alert{false}, eepromWrite{false}, statusArray{{27, 1}, {26, 1}, {25, 1}, {33, 1}, {32, 1}, {17, 1}, {18, 1}, {19, 1}}, buttonUp{0}, buttonDown{0}, buttonBack{0}, buttonOK{0} {};

    void setCurrentTime(String time)
    {
        currentTime = time;
    }

    String getCurrentTime()
    {
        return currentTime;
    }

    void setAlert(String _alert)
    {
        alert = _alert;
    }

    String getAlert()
    {
        return alert;
    }

    void setEepromWrite(bool eWrite)
    {
        eepromWrite = eWrite;
    }

    bool getEepromWrite()
    {
        return eepromWrite;
    }

    void setStatusArray(int col, int row, int value)
    {
        statusArray[col][row] = value;
    }

    int getStatusArray(int col, int row)
    {
        return statusArray[col][row];
    }

    String getEquipmentStatus(int col)
    {
        if (statusArray[col][1] == 1)
        {
            return String(" - On ");
        }
        else
        {
            return String(" - Off");
        }
    }
};

extern struct tm timeinfo;

void inputMonitor();
void tempControl();
void equipmentControl();

void doWiFi(void *parameter);
void webServer();
void optionsMenu();
void menuChange();

void setRTC();
void timeMonitor();
void lightControl();
void webServer();

#endif
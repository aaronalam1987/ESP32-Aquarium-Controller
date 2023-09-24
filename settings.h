#ifndef SETTINGS_H
#define SETTINGS_H
#include <Preferences.h>

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

    Preferences &getConfig()
    {
        return config;
    }
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

#endif
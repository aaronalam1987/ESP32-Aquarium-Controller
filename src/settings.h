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
    bool enableLightCon;
    bool enableTempCon;
    bool enableATO;
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

    Settings() : targetTemp{0}, tempVariant{0}, tempAlert{0}, lightOn{0}, lightOff{0}, enableLightCon{false}, enableTempCon{false}, enableATO{false}, wifiSsid{""}, wifiPassword{""}, deviceOneName{""}, deviceTwoName{""}, deviceThreeName{""}, deviceFourName{""}, deviceFiveName{""}, deviceSixName{""}, deviceSevenName{""}, deviceEightName{""} {};

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
        enableLightCon = config.getBool("enableLightcon", false);
        enableTempCon = config.getBool("enableTempCon", false);
        enableATO = config.getBool("enableATO", false);
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
        config.putBool("enableLightCon", enableLightCon);
        config.putBool("enableTempCon", enableTempCon);
        config.putBool("enableATO", enableATO);
    }
};

#endif
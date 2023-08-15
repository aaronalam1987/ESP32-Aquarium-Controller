#include "global.h"
#include <Preferences.h>
Preferences config;
extern struct settings Settings;

// Read the config file created by Parameters library and assign the values to the setting struct.
void readEeprom()
{
    config.begin("config", false);
    Settings.targetTemp = config.getDouble("targetTemp", 0);
    Settings.tempVariant = config.getDouble("tempVariant", 0);
    Settings.tempAlert = config.getDouble("tempAlert", 0);
    Settings.lightOn = config.getDouble("lightOn", 0);
    Settings.lightOff = config.getDouble("lightOff", 0);
    Settings.wifiSsid = config.getString("wifiSsid", "");
    Settings.wifiPassword = config.getString("wifiPassword", "");
}

// Options menu sets writeEeprom to true, using the assumption that settings may have been changed.
// Write settings to eeprom on init page and change writeEeprom to false to prevent constant saving.
void writeEeprom()
{
    if (currentMenu == 0 && eepromWrite)
    {
        config.putDouble("targetTemp", Settings.targetTemp);
        config.putDouble("tempVariant", Settings.tempVariant);
        config.putDouble("tempAlert", Settings.tempAlert);
        config.putDouble("lightOn", Settings.lightOn);
        config.putDouble("lightOff", Settings.lightOff);
        config.putString("wifiSsid", Settings.wifiSsid);
        config.putString("wifiPassword", Settings.wifiPassword);
        eepromWrite = false;
    }
}
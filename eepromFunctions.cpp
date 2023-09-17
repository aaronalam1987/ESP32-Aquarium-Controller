#include "global.h"

extern Settings settings;
// extern struct settings Settings;

// Read the config file created by Parameters library and assign the values to the setting struct.
void readEeprom()
{
}

// Options menu sets writeEeprom to true, using the assumption that settings may have been changed.
// Write settings to eeprom on init page and change writeEeprom to false to prevent constant saving.
void writeEeprom()
{
    // if (currentMenu == 0 && eepromWrite)
    // {

    //     eepromWrite = false;
    // }
    // Read eeprom to update values that do not directly access eeprom.
    // readEeprom();
}
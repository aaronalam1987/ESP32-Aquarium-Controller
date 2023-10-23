#include "global.h"
#include "settings.h"
#include "menuControl.h"
#include "espNow.h"
#include <map>

extern Settings settings;
extern Menus menu;
extern System sys;
extern espNow espnow;

void equipmentControl()
{
    // Equipment Control allows for controlling either relays associated with the master device or controlling a slave device,
    // utilising ESP-NOW
    //  Map the currently selected menu line to the associated GPIO.
    std::map<int, int> gpioMap;
    gpioMap[menuLineOne] = 27;   // Relay One.
    gpioMap[menuLineTwo] = 26;   // Relay Two.
    gpioMap[menuLineThree] = 25; // Relay Three.
    gpioMap[menuLineFour] = 33;  // Relay Four.

    // statusArray allows us to associate a value to each gpio pin allowing us to know if the device is off (0) or on (1)
    sys.setStatusArray(0, 0, gpioMap[menuLineOne]);
    sys.setStatusArray(1, 0, gpioMap[menuLineTwo]);
    sys.setStatusArray(2, 0, gpioMap[menuLineThree]);
    sys.setStatusArray(3, 0, gpioMap[menuLineFour]);

    // Clear menu.
    menu.clearMenu();

    // Build our menu.
    menu.setMenu(0, menuLineOne, settings.deviceOneName + sys.getEquipmentStatus(menuLineOne));
    menu.setMenu(0, menuLineTwo, settings.deviceTwoName + sys.getEquipmentStatus(menuLineTwo));
    menu.setMenu(0, menuLineThree, settings.deviceThreeName + sys.getEquipmentStatus(menuLineThree));
    menu.setMenu(0, menuLineFour, settings.deviceFourName + sys.getEquipmentStatus(menuLineFour));

    // Is selectable.
    menu.menuSelectable(true);

    // Low indicates button is pressed.
    if (sys.buttonOK == LOW)
    {
        for (int i = 0; i < 4; i++)
        {
            // This allows us to alternate specified relay on or off.
            // Automated elements take priority over manual input.
            if (sys.getStatusArray(i, 0) == gpioMap[menu.getMenuSelect()])
            {
                // StatusArray is only set to 0 (OFF) or 1 (ON).
                // As we are switching between two states only, we can define that if it is set to 0 (OFF) switch to 1 (ON).
                if (sys.getStatusArray(i, 1) == OFF)
                {
                    sys.setStatusArray(i, 1, ON);
                }
                else
                {
                    // Is not set to 0 (OFF) so we can assume it is currently set to 1 (ON), set to 0 (OFF).
                    sys.setStatusArray(i, 1, OFF);
                }
                // Turns local relay on/off based on statusArray value (0 / 1);
                digitalWrite(gpioMap[menu.getMenuSelect()], sys.getStatusArray(i, 1));
            }
        }
        // Updates slave device with new relay values.
        espnow.sendData();
    }
}
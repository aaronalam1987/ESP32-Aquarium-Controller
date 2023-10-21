#include "global.h"
#include "settings.h"
#include "menuControl.h"
#include "espnow.h"
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
    gpioMap[menuLineOne] = 27;
    gpioMap[menuLineTwo] = 26;
    gpioMap[menuLineThree] = 25;
    gpioMap[menuLineFour] = 33;

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
            if (sys.getStatusArray(i, 0) == gpioMap[menu.getMenuSelect()])
            {
                // StatusArray is only set to 0 (gpioOFF) or 1 (gpioON).
                // As we are switching between two states only, we can define that if it is set to 0 (gpioOFF) switch to 1 (gpioON).
                if (sys.getStatusArray(i, 1) == gpioOFF)
                {
                    sys.setStatusArray(i, 1, gpioON);
                }
                else
                {
                    // Is not set to 0 (gpioOFF) so we can assume it is currently set to 1 (gpioON), set to 0 (gpioOFF).
                    sys.setStatusArray(i, 1, gpioOFF);
                }
                // Turns local relay on/off based on statusArray value (0 / 1);
                digitalWrite(gpioMap[menu.getMenuSelect()], sys.getStatusArray(i, 1));
            }
        }
        // Updates slave device with new relay values.
        espnow.sendData();
    }
}
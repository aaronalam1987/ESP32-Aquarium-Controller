#include "global.h"
#include <map>

extern Settings settings;
extern Menus menu;
extern System sys;

void equipmentControl()
{
    // Map the currently selected menu line to the associated GPIO.
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

    // Clear and build menu.
    menu.clearMenu();
    menu.setMenu(0, menuLineOne, settings.deviceOneName + sys.getEquipmentStatus(menuLineOne));
    menu.setMenu(0, menuLineTwo, settings.deviceTwoName + sys.getEquipmentStatus(menuLineTwo));
    menu.setMenu(0, menuLineThree, settings.deviceThreeName + sys.getEquipmentStatus(menuLineThree));
    menu.setMenu(0, menuLineFour, settings.deviceFourName + sys.getEquipmentStatus(menuLineFour));
    menu.menuSelectable(true);

    if (buttonOK == LOW)
    {
        for (int i = 0; i < 4; i++)
        {
            // This allows us to alternate specified relay on or off.
            if (sys.getStatusArray(i, 0) == gpioMap[menu.getMenuSelect()])
            {
                if (sys.getStatusArray(i, 1) == 0)
                {
                    sys.setStatusArray(i, 1, 1);
                }
                else
                {
                    sys.setStatusArray(i, 1, 0);
                }
                digitalWrite(gpioMap[menu.getMenuSelect()], sys.getStatusArray(i, 1));
            }
        }
    }
}
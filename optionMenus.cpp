#include "global.h"
#include <map>
extern Menus menu;

void menuChange(int currMenu, int prevMenu)
{
    // Tiny but useful function, used to navigate options menus.
    // currentMenu = currMenu;
    // previousMenu = prevMenu;
}

void optionsMenu()
{
    // Set our menu.
    menu.setMenu(0, 0, "Equipment Control   ");
    menu.setMenu(0, 1, "Temperature Control ");
    menu.setMenu(0, 2, "Light Timer Settings");
    menu.setMenu(0, 3, "Settings (via WiFi) ");
    // Is selectable.
    menu.menuSelectable(true);

    // Build a map to link our current "menuSelect" to it's associated option.
    std::map<int, int> optionsMap;
    optionsMap[0] = 2; // Equipment Control.
    optionsMap[1] = 3; // Temperature Control.
    optionsMap[2] = 4; // Light Timer Settings.
    optionsMap[3] = 5; // Settings (via WiFi).

    // buttonOK has been pressed, set our currentMenu using the associated value from the optionsMap.
    if (buttonOK == LOW)
    {
        menu.clearMenu();
        menu.setCurrentMenu(optionsMap[menu.getMenuSelect()]);
    }
}
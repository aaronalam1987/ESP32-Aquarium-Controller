#include "global.h"

void menuChange(int currMenu, int prevMenu)
{
    // Tiny but useful function, used to navigate options menus.
    currentMenu = currMenu;
    previousMenu = prevMenu;
}

void optionsMenu()
{
    selectableMenu = true;
    eepromWrite = true;
    previousMenu = 0;
    if (buttonDown == LOW && menuSelect == 3)
    {
        menuSelect = 0;
        menuChange(6, 1);
    }

    if (buttonOK == LOW)
    {
        switch (menuSelect)
        {
        case 0:
            menuChange(2, 1);
            break;
        case 1:
            menuChange(3, 1);
            break;
        case 2:
            menuChange(4, 1);
            break;
        case 3:
            menuChange(5, 1);
            break;
        }
    }
    Serial.println(menuSelect);
}

void optionsMenuMore()
{
    selectableMenu = true;
    if (buttonUp == LOW && menuSelect == 0)
    {
        currentMenu = 1;
        menuSelect = 3;
    }

    if (buttonOK == LOW)
    {
        switch (menuSelect)
        {
        case 1:
            menuChange(8, 6);
            break;
        }
    }
    Serial.println(menuSelect);
}
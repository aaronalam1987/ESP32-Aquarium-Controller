#include "global.h"
extern TaskHandle_t doWifi;
extern Menus menu;
void inputMonitor()
{
    // inputMonitor increments or decrements the currently selected menu line.
    //  buttonUp press without buttonOK press, if we don't check that buttonOK is not pressed, we lose the ability to increment specific elements on menu pages.
    if (buttonDown == LOW && buttonOK != LOW)
    {
        if (menu.getMenuSelect() <= menuLineFour)
        {
            menu.setMenuSelect(menu.getMenuSelect() + 1);
        }
    }

    if (buttonUp == LOW && buttonOK != LOW)
    {
        if (menu.getMenuSelect() > menuLineOne)
        {
            menu.setMenuSelect(menu.getMenuSelect() - 1);
        }
    }

    if (buttonBack == LOW)
    {
        menu.setCurrentMenu(menuInit);
    }
}
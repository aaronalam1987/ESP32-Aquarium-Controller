#include "global.h"
#include "menuControl.h"

extern System sys;
extern Menus menu;
void inputMonitor()
{
    // inputMonitor increments or decrements the currently selected menu line.
    //  buttonUp press without buttonOK press, if we don't check that buttonOK is not pressed, we lose the ability to increment specific elements on menu pages.
    if (sys.buttonDown == LOW && sys.buttonOK != LOW)
    {
        if (menu.getMenuSelect() <= menuLineFour)
        {
            menu.setMenuSelect(menu.getMenuSelect() + 1);
        }
    }

    if (sys.buttonUp == LOW && sys.buttonOK != LOW)
    {
        if (menu.getMenuSelect() > menuLineOne)
        {
            menu.setMenuSelect(menu.getMenuSelect() - 1);
        }
    }

    if (sys.buttonBack == LOW)
    {
        menu.setCurrentMenu(menuInit);
    }
}
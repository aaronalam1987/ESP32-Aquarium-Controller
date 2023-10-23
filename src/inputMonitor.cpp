#include "global.h"
#include "menuControl.h"

extern System sys;
extern Menus menu;
void inputMonitor()
{
    // inputMonitor increments or decrements the currently selected menu line.
    // Check for buttonUp/Down press without buttonOK press, if we don't check that buttonOK is not pressed, we lose the ability to increment specific elements on menu pages.
    if (sys.buttonDown == LOW && sys.buttonOK != LOW)
    {
        // Current menuSelect is less menuLineFour (which is for int 3).
        // Increment menuSelect.
        if (menu.getMenuSelect() < menuLineFour)
        {
            menu.setMenuSelect(menu.getMenuSelect() + 1);
        }
    }

    if (sys.buttonUp == LOW && sys.buttonOK != LOW)
    {
        // Current menuSelect is greated than menuLineOne (for int, 0).
        // Decrement menuSelect.
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
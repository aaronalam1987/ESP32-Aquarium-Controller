#include "global.h"
extern TaskHandle_t doWifi;
extern Menus menu;
void inputMonitor()
{
    // menuChanged is used to identify if a new menu page has been accessed, it is compared to current menu to allow us to track if the lcd should be cleared.
    /* if (menu.getMenuChanged() != menu.getCurrentMenu())
     {
         // Clear lcd, we avoid this as much as possible but upon menu change, it is ideal.
         // Switch menuChanged to currentMenu to keep track
         // lcd.clear();
         menu.setMenuSelect(0);
         menu.clearMenu();
         menu.setMenuChanged();
         // menuSelect keeps track of which line of the lcd is "active", for all menus but options menu, we reset to 0 which is the first line.
     }*/

    // buttonUp press without buttonOK press, if we don't check that buttonOK is not pressed, we lose the ability to increment specific elements on menu pages.
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
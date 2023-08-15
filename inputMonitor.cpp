#include "global.h"
extern TaskHandle_t doWifi;
void inputMonitor()
{
    // menuChanged is used to identify if a new menu page has been accessed, it is compared to current menu to allow us to track if the lcd should be cleared.
    if (menuChanged != currentMenu)
    {
        // Clear lcd, we avoid this as much as possible but upon menu change, it is ideal.
        // Switch menuChanged to currentMenu to keep track
        lcd.clear();
        menuChanged = currentMenu;
        if (currentMenu != 1 || currentMenu != 6)
        {
            // menuSelect keeps track of which line of the lcd is "active", for all menus but options menu, we reset to 0 which is the first line.
            menuSelect = 0;
        }
    }

    // buttonUp press without buttonOK press, if we don't check that buttonOK is not pressed, we lose the ability to increment specific elements on menu pages.
    if (buttonDown == LOW && buttonOK != LOW)
    {
        if (currentMenu != 1)
        {
            // Menu "1" is the first options page, as long we are not here, this range works for navigating options.
            menuSelect <= 2 ? menuSelect++ : menuSelect = menuSelect;
        }
        else
        {
            // Menu is "6" / first options page, allow a high increment to allow us to "scroll" from the first options page to the second.
            menuSelect <= 3 ? menuSelect++ : menuSelect = menuSelect;
        }
        // Simple function to clear the previous line as we scroll through the menu.
        clearLine(menuSelect - 1);
    }

    if (buttonUp == LOW && buttonOK != LOW)
    {
        // As above but in reverse.
        if (currentMenu != 6)
        {
            menuSelect > 0 ? menuSelect-- : menuSelect = menuSelect;
        }
        else
        {
            menuSelect > -1 ? menuSelect-- : menuSelect = menuSelect;
        }
        clearLine(menuSelect + 1);
    }

    if (buttonBack == LOW)
    {
        currentMenu = previousMenu;
    }
}
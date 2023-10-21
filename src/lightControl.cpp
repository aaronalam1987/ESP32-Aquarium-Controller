#include "global.h"
#include "settings.h"
#include "menuControl.h"
extern Settings settings;
extern System sys;
extern Menus menu;

void lightControl()
{
    menu.menuSelectable(true);
    // OK button and either UP or DOWN button increments and decrements the value specified.
    if (sys.buttonOK == LOW && sys.buttonUp == LOW)
    {
        switch (menu.getMenuSelect())
        {
        case 0:
            settings.lightOn += 0.1;
            break;
        case 1:
            settings.lightOff += 0.1;
            break;
        }
    }

    if (sys.buttonOK == LOW && sys.buttonDown == LOW)
    {
        switch (menu.getMenuSelect())
        {
        case 0:
            settings.lightOn -= 0.1;
            break;
        case 1:
            settings.lightOff -= 0.1;
            break;
        }
    }

    if (sys.buttonOK == LOW && menu.getMenuSelect() == 3)
    {
        // OK press on menuSelect 3 is "save".
        menu.setCurrentMenu(0);
    }
}
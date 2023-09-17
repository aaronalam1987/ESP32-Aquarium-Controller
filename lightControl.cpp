#include "global.h"
extern Settings settings;
extern Menus menu;

void lightControl()
{
    menu.menuSelectable(true);
    // OK button and either UP or DOWN button increments and decrements the value specified.
    if (buttonOK == LOW && buttonUp == LOW)
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

    if (buttonOK == LOW && buttonDown == LOW)
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

    if (buttonOK == LOW && menu.getMenuSelect() == 3)
    {
        // OK press on menuSelect 3 is "save".
        menu.setCurrentMenu(0);
    }
}
#include "global.h"

void drawMenu()
{
    // Simple function to draw the menu based off of the currentMenu value and the menuArray.
    // If "selectableMenu" is true, append the beginning of the string with ">".
    for (int i = 0; i <= 3; i++)
    {
        if (menuSelect == i && selectableMenu && menuArray[currentMenu][i] != "")
        {
            selectIndicator = ">";
        }
        lcd.setCursor(0, i);
        lcd.print(selectIndicator + menuArray[currentMenu][i]);
        // Reset selectIndicator to "" to prevent every option having this appended.
        selectIndicator = "";
    }
}

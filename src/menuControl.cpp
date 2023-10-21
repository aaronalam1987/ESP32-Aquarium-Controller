#include "global.h"
#include "menuControl.h"
Menus menu;
LiquidCrystal_I2C &lcd = menu.getLCD();

void Menus::lcdInit()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();
}

// clearMenu is used to identify if the current menu has been changed, if so, clear the lcd.
// We do it this way rather than calling lcd.clear constantly as it introduces flicker.
void Menus::clearMenu()
{
    if (menu.getMenuChanged() != menu.getCurrentMenu())
    {
        menu.setMenuChanged();
        lcd.clear();
    }
}

// drawMenu is responsible for drawing the currently selected menu page and checking whether or not it is a selectable menu.
// if so, it adds ">" to the start of the line.
void Menus::drawMenu()
{
    String selected = "";
    for (int i = 0; i <= 3; i++)
    {
        if (menu.getMenuSelect() == i && menu.isSelectable())
        {
            selected = ">";
        }
        while (menu.getMenuArr(0, i).length() < 20)
        {
            menu.setMenu(0, i, menu.getMenuArr(0, i) + " ");
        }
        lcd.setCursor(0, i);
        lcd.print(selected + menu.getMenuArr(0, i));
        selected = "";
    }
}
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
void Menus::clearMenu()
{
    if (menu.getMenuChanged() != menu.getCurrentMenu())
    {
        menu.setMenuChanged();
        lcd.clear();
    }
}
void Menus::drawMenu()
{
    String selected = "";
    for (int i = 0; i <= 3; i++)
    {
        if (menu.getMenuSelect() == i && menu.isSelectable())
        {
            selected = ">";
        }
        lcd.setCursor(0, i);
        lcd.print(selected + menu.getMenuArr(0, i));
        selected = "";
    }
}
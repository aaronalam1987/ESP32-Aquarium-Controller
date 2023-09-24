#ifndef MENUS_H
#define MENUS_H
#include <LiquidCrystal_I2C.h>

class Menus
{

private:
    LiquidCrystal_I2C lcd;
    String menuArray[1][4];
    int currentMenu;
    int menuChanged;
    int menuSelect;
    bool selectableMenu;

public:
    Menus() : lcd(0x27, 20, 4), menuArray{""}, currentMenu{0}, menuChanged{0}, selectableMenu{false} {}
    String getMenuArr(int col, int row)
    {
        return menuArray[col][row];
    }
    int getCurrentMenu()
    {
        return currentMenu;
    }
    int getMenuChanged()
    {
        return menuChanged;
    }
    void setMenuChanged()
    {
        menuChanged = currentMenu;
    }
    int getMenuSelect()
    {
        return menuSelect;
    }
    void setMenuSelect(int setMenuSelect)
    {
        menuSelect = setMenuSelect;
    }
    void setCurrentMenu(int currMenu)
    {
        currentMenu = currMenu;
    }
    void setMenu(int col, int row, String menuText)
    {
        menuArray[col][row] = menuText;
    }
    void menuSelectable(bool selectable)
    {
        selectableMenu = selectable;
    }
    bool isSelectable()
    {
        return selectableMenu;
    }

    LiquidCrystal_I2C &getLCD()
    {
        return lcd;
    }

    void lcdInit();

    void clearMenu();

    void drawMenu();
};

#endif
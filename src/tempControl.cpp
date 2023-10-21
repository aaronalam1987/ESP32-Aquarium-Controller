#include "global.h"
#include "settings.h"
#include "menuControl.h"
extern Settings settings;
extern Menus menu;
extern System sys;
void tempControl()
{
  // Clear menu.
  menu.clearMenu();

  // Build our menu.
  menu.setMenu(0, menuLineOne, "Target Temp: " + String(settings.targetTemp));
  menu.setMenu(0, menuLineTwo, "Temp Variant: " + String(settings.tempAlert));
  menu.setMenu(0, menuLineThree, "Temp Alert: " + String(settings.tempAlert));
  menu.setMenu(0, menuLineFour, "Save                ");

  // Is selectable.
  menu.menuSelectable(true);

  // OK button and either UP or DOWN button increments and decrements the value specified.
  if (sys.buttonOK == LOW && sys.buttonUp == LOW)
  {
    switch (menu.getMenuSelect())
    {
    case 0:
      settings.targetTemp += 0.1;
      break;
    case 1:
      settings.tempVariant += 0.1;
      break;
    case 2:
      settings.tempAlert += 0.1;
      break;
    }
  }

  if (sys.buttonOK == LOW && sys.buttonDown == LOW)
  {
    switch (menu.getMenuSelect())
    {
    case 0:
      settings.targetTemp -= 0.1;
      break;
    case 1:
      settings.tempVariant -= 0.1;
      break;
    case 2:
      settings.tempAlert -= 0.1;
      break;
    }
  }

  if (sys.buttonOK == LOW && menu.getMenuSelect() == menuLineFour)
  {
    // OK press on menuLineFour is "save".
    menu.setCurrentMenu(menuInit);
  }
}
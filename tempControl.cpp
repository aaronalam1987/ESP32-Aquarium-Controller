#include "global.h"
extern Settings settings;
extern Menus menu;
void tempControl()
{
  menu.menuSelectable(true);
  menu.setMenu(0, 0, "Target Temp: " + String(settings.targetTemp));
  menu.setMenu(0, 1, "Temp Variant: " + String(settings.tempAlert));
  menu.setMenu(0, 2, "Temp Alert: " + String(settings.tempAlert));
  menu.setMenu(0, 3, "Save                ");
  // OK button and either UP or DOWN button increments and decrements the value specified.
  if (buttonOK == LOW && buttonUp == LOW)
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

  if (buttonOK == LOW && buttonDown == LOW)
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

  if (buttonOK == LOW && menu.getMenuSelect() == menuLineFour)
  {
    // OK press on menuLineFour is "save".
    menu.setCurrentMenu(menuInit);
  }
}
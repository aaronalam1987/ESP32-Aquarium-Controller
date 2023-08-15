#include "global.h"
extern struct settings Settings;

void tempControl()
{
  selectableMenu = true;
  // OK button and either UP or DOWN button increments and decrements the value specified.
  if (buttonOK == LOW && buttonUp == LOW)
  {
    switch (menuSelect)
    {
    case 0:
      Settings.targetTemp += 0.1;
      break;
    case 1:
      Settings.tempVariant += 0.1;
      break;
    case 2:
      Settings.tempAlert += 0.1;
      break;
    }
  }

  if (buttonOK == LOW && buttonDown == LOW)
  {
    switch (menuSelect)
    {
    case 0:
      Settings.targetTemp -= 0.1;
      break;
    case 1:
      Settings.tempVariant -= 0.1;
      break;
    case 2:
      Settings.tempAlert -= 0.1;
      break;
    }
  }

  if (buttonOK == LOW && menuSelect == 3)
  {
    // OK press on menuSelect 3 is "save".
    currentMenu = 0;
  }

  // Convert targetTemp to string and amend our menu array.
  menuArray[3][0] = "Target Temp: " + String(Settings.targetTemp).substring(0, 4);
  menuArray[3][1] = "Temp Variant: " + String(Settings.tempVariant).substring(0, 4);
  menuArray[3][2] = "Temp Alert: " + String(Settings.tempAlert).substring(0, 4);
}
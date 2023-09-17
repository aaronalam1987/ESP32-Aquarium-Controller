#include "global.h"

extern Settings settings;
extern Menus menu;
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

TempMonitor tempMonitor(&oneWire, &tempSensor);
void monitorTempFunction(void *parameter)
{
    // Initialise temperature probe.
    tempMonitor.initialise();
    for (;;)
    {
        tempMonitor.checkCurrentTemp();
        // Convert to string and assign currentTemp value to the menuArray

        // Check to see if temperature is higher than the variant allows for.
        // If so, disable heating relay and enable cooling.
        if (tempMonitor.getCurrentTemp() - settings.targetTemp >= settings.tempVariant)
        {
            // If currentMenu is set to 2, we don't want to act as this menu allows control over connected equipment.
            // Automatic control will resume once this menu is not active.
            if (menu.getCurrentMenu() != equipmentMenu)
            {
                digitalWrite(settings.heatingGPIO, gpioOFF);
                digitalWrite(settings.coolingGPIO, gpioON);
                tempMonitor.setMode("Cool");
            }
        }

        // Check to see if temperature is lower than the variant allows for.
        // If so, disable cooling relay and enable heating.
        else if (settings.targetTemp - tempMonitor.getCurrentTemp() >= settings.tempVariant)
        {
            if (menu.getCurrentMenu() != equipmentMenu)
            {
                digitalWrite(settings.heatingGPIO, gpioON);
                digitalWrite(settings.coolingGPIO, gpioOFF);
                tempMonitor.setMode("Heat");
            }
        }
        // Temperature is in range of variant, disable heating and cooling and set mode to "Off".
        else
        {
            if (menu.getCurrentMenu() != equipmentMenu)
            {
                digitalWrite(settings.heatingGPIO, gpioOFF);
                digitalWrite(settings.coolingGPIO, gpioOFF);
                tempMonitor.setMode("Off");
            }
        }

        // Check temperature compared to alert range, assign appropriate alerts if needed.
        if (tempMonitor.getCurrentTemp() - settings.targetTemp >= settings.tempAlert || settings.targetTemp - tempMonitor.getCurrentTemp() >= settings.tempAlert)
        {
            // menuArray[0][2] = "Alerts:Temp Warning!";
            alert = true;
        }
        else
        {
            // menuArray[0][2] = "Alerts:None         ";
            alert = false;
        }
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
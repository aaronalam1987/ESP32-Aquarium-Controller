#include "global.h"
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);
extern struct settings Settings;
double currentTemp;
String mode;

void monitorTempFunction(void *parameter)
{
    // Start tempSensor, initilised by DallasTemperature library.
    tempSensor.begin();
    for (;;)
    {
        // Get current temperature from probe and assign to double.
        tempSensor.requestTemperatures();
        currentTemp = tempSensor.getTempCByIndex(0);

        // Convert to string and assign currentTemp value to the menuArray
        menuArray[0][0] = "Temp:" + String(currentTemp).substring(0, 4) + " Mode:" + mode;

        // Check to see if temperature is higher than the variant allows for.
        // If so, disable heating relay and enable cooling.
        if (currentTemp - Settings.targetTemp >= Settings.tempVariant)
        {
            // Serial.println("disable heat, enable cool");
            // If currentMenu is set to 2, we don't want to act as this menu allows control over connected equipment.
            // Automatic control will resume once this menu is not active.
            if (currentMenu != 2)
            {
                digitalWrite(Settings.heatingGPIO, 0);
                digitalWrite(Settings.coolingGPIO, 1);
                mode = "Cool";
            }
        }

        // Check to see if temperature is lower than the variant allows for.
        // If so, disable cooling relay and enable heating.
        else if (Settings.targetTemp - currentTemp >= Settings.tempVariant)
        {
            // Serial.println("enable heat, disable cool");
            if (currentMenu != 2)
            {
                digitalWrite(Settings.heatingGPIO, 1);
                digitalWrite(Settings.coolingGPIO, 0);
                mode = "Heat";
            }
        }
        // Temperature is in range of variant, disable heating and cooling and set mode to "Off".
        else
        {
            if (currentMenu != 2)
            {
                digitalWrite(Settings.heatingGPIO, 0);
                digitalWrite(Settings.coolingGPIO, 0);
                mode = "Off ";
            }
        }

        // Check temperature compared to alert range, assign appropriate alerts if needed.
        if (currentTemp - Settings.targetTemp >= Settings.tempAlert || Settings.targetTemp - currentTemp >= Settings.tempAlert)
        {
            menuArray[0][2] = "Alerts:Temp Warning!";
            alert = true;
        }
        else
        {
            menuArray[0][2] = "Alerts:None         ";
            alert = false;
        }
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
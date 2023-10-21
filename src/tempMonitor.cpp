#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "menuControl.h"

extern Settings settings;
extern Menus menu;
extern System sys;

OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

TempMonitor tempMonitor(&oneWire, &tempSensor);
void monitorTempFunction(void *parameter)
{
    // Initialise temperature probe.
    tempMonitor.initialise();
    for (;;)
    {
        // checkCurrentTemp requests updated temperature and assigns it to currentTemp double.
        tempMonitor.checkCurrentTemp();

        // Check to see if temperature is higher than the variant allows for.
        // If so, disable heating relay and enable cooling.
        if (tempMonitor.getCurrentTemp() - settings.targetTemp >= settings.tempVariant)
        {
            // This checks to see if the GPIO set during configuration matches a GPIO in our statusArray, if so, it disables heating
            // and looks for the matching cooling gpio which is then enabled, essentially this just allows us to display "On" or "Off" in the menus.
            for (int i = 0; i < 8; i++)
            {
                if (sys.getStatusArray(i, 0) == settings.heatingGPIO)
                {
                    sys.setStatusArray(i, 1, gpioOFF);
                    for (int i = 0; i < 8; i++)
                    {
                        if (sys.getStatusArray(i, 0) == settings.coolingGPIO)
                        {
                            sys.setStatusArray(i, 1, gpioON);
                        }
                    }
                }
                // This is responsible for switching the relays on or off.
                digitalWrite(settings.heatingGPIO, gpioOFF);
                digitalWrite(settings.coolingGPIO, gpioON);
                tempMonitor.setMode("Cool");
            }
        }

        // Check to see if temperature is lower than the variant allows for.
        // If so, disable cooling relay and enable heating.
        else if (settings.targetTemp - tempMonitor.getCurrentTemp() >= settings.tempVariant)
        {
            for (int i = 0; i < 8; i++)
            {
                if (sys.getStatusArray(i, 0) == settings.coolingGPIO)
                {
                    sys.setStatusArray(i, 1, gpioOFF);
                    for (int i = 0; i < 8; i++)
                    {
                        if (sys.getStatusArray(i, 0) == settings.heatingGPIO)
                        {
                            sys.setStatusArray(i, 1, gpioON);
                        }
                    }
                }
            }

            digitalWrite(settings.heatingGPIO, gpioON);
            digitalWrite(settings.coolingGPIO, gpioOFF);
            tempMonitor.setMode("Heat");
        }
        // Temperature is in range of variant, disable heating and cooling and set mode to "Off".
        else
        {
            for (int i = 0; i < 8; i++)
            {
                if (sys.getStatusArray(i, 0) == settings.heatingGPIO || settings.coolingGPIO)
                {
                    sys.setStatusArray(i, 1, gpioOFF);
                }
            }

            digitalWrite(settings.heatingGPIO, gpioOFF);
            digitalWrite(settings.coolingGPIO, gpioOFF);
            tempMonitor.setMode("Off");
        }

        // Check temperature compared to alert range, assign appropriate alerts if needed.
        if (tempMonitor.getCurrentTemp() - settings.targetTemp >= settings.tempAlert || settings.targetTemp - tempMonitor.getCurrentTemp() >= settings.tempAlert)
        {
            sys.setAlert("Temp Warning!");
        }
        else
        {
            sys.setAlert("None");
        }
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
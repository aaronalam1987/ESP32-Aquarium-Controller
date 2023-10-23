#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "menuControl.h"
#include "espnow.h"

extern Settings settings;
extern Menus menu;
extern System sys;
extern espNow espnow;

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
        if (settings.enableTempCon)
        {
            // Check to see if temperature is higher than the variant allows for.
            // If so, disable heating relay and enable cooling.
            if (tempMonitor.getCurrentTemp() - settings.targetTemp >= settings.tempVariant)
            {
                /*
                ------------------------------------------------------------------------
                This code is defunct as heating and cooling GPIO are no longer configurable and are required to be at pin 26/27
                ------------------------------------------------------------------------
                This checks to see if the GPIO set during configuration matches a GPIO in our statusArray, if so, it disables heating
                and looks for the matching cooling gpio which is then enabled, essentially this just allows us to display "On" or "Off" in the menus.
                for (int i = 0; i < 8; i++)
                {
                    if (sys.getStatusArray(i, 0) == settings.heatingGPIO)
                    {
                        sys.setStatusArray(i, 1, OFF);
                        for (int i = 0; i < 8; i++)
                        {
                            if (sys.getStatusArray(i, 0) == settings.coolingGPIO)
                            {
                                sys.setStatusArray(i, 1, ON);
                            }
                        }
                    }
                    // This is responsible for switching the relays on or off.
                    digitalWrite(settings.heatingGPIO, OFF);
                    digitalWrite(settings.coolingGPIO, ON);
                    tempMonitor.setMode("Cool");
                }
                */
                digitalWrite(27, OFF);
                digitalWrite(26, ON);
                sys.setStatusArray(0, 1, OFF);
                sys.setStatusArray(1, 1, ON);
                tempMonitor.setMode("Cool");
                espnow.sendData();
            }

            // Check to see if temperature is lower than the variant allows for.
            // If so, disable cooling relay and enable heating.
            else if (settings.targetTemp - tempMonitor.getCurrentTemp() >= settings.tempVariant)
            {
                digitalWrite(27, ON);
                digitalWrite(26, OFF);
                sys.setStatusArray(0, 1, ON);
                sys.setStatusArray(1, 1, OFF);
                tempMonitor.setMode("Heat");
                espnow.sendData();
            }
            // Temperature is in range of variant, disable heating and cooling and set mode to "Off".
            else
            {
                digitalWrite(26, OFF);
                digitalWrite(27, OFF);
                sys.setStatusArray(0, 1, OFF);
                sys.setStatusArray(1, 1, OFF);
                tempMonitor.setMode("Off");
                espnow.sendData();
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
        }
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
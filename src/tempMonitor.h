#ifndef TEMPMONITOR_H
#define TEMPMONITOR_H

#include <oneWire.h>
#include <DallasTemperature.h>

class TempMonitor
{
private:
    OneWire *oneWire;
    DallasTemperature *tempSensor;
    double currentTemp;
    double temperatureLog[2][24];
    String currentMode;

public:
    TempMonitor(OneWire *wire, DallasTemperature *sensor) : oneWire(wire), tempSensor(sensor), temperatureLog{{0}}, currentTemp{0}, currentMode("DISD") {}
    void initialise()
    {
        tempSensor->begin();
    }
    void checkCurrentTemp()
    {
        tempSensor->requestTemperatures();
        currentTemp = tempSensor->getTempCByIndex(0);
    }
    double getCurrentTemp()
    {
        return currentTemp;
    }
    void setTempLog(int col, int row, double entry)
    {
        temperatureLog[col][row] = entry;
    }
    double getTempLog(int col, int row)
    {
        return temperatureLog[col][row];
    }
    String getCurrentMode()
    {
        return currentMode;
    }
    void setMode(String mode)
    {
        currentMode = mode;
    }
};
void monitorTempFunction(void *parameter);
#endif
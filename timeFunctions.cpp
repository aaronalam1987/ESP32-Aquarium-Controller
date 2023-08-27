#include "global.h"
#include <time.h>
#include <ESP32Time.h>

const char *ntpServer = "pool.ntp.org";
const long gmtOffSet = 0;
const int daylightSavingsOffset = 3600;
char fullTime[8];
ESP32Time rtc(0);
struct tm timeinfo;
int timeChange = 0;

void setRTC()
{
    configTime(gmtOffSet, daylightSavingsOffset, ntpServer);
    if (getLocalTime(&timeinfo))
    {
        rtc.setTimeStruct(timeinfo);
        timeChange = rtc.getHour(true);
    }
}

void timeMonitor()
{

    if (timeChange != rtc.getHour(true))
    {
        // Serial.println("Time changed");
        //  We have passed an hour, update double temp log.
        tempLog[rtc.getHour(true)] = currentTemp;
        // Serial.println(tempLog[rtc.getHour(true)]);
        timeChange = rtc.getHour(true);
    }
    // Time is midnight, reset RTC from NTP pool and clear logged temperatures.
    if (rtc.getHour() == '0')
    {
        // Serial.println("its midnight?");
        tempLog[24] = {0};
        setRTC();
    }
    else
    {
        // set temp array.
    }
    menuArray[0][3] = "Time: " + rtc.getTime();
}
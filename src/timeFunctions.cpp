#include "global.h"
#include "tempMonitor.h"
#include <time.h>
#include <ESP32Time.h>

const char *ntpServer = "pool.ntp.org";
const long gmtOffSet = 0;
const int daylightSavingsOffset = 3600;
ESP32Time rtc(0);
struct tm timeinfo;
int timeChange{0}, dayChange{0};
extern TempMonitor tempMonitor;
extern System sys;

void setRTC()
{
    configTime(gmtOffSet, daylightSavingsOffset, ntpServer);
    if (getLocalTime(&timeinfo))
    {
        rtc.setTimeStruct(timeinfo);
        timeChange = rtc.getHour(true);
        dayChange = rtc.getDayofWeek();
    }
}

void timeMonitor()
{
    if (timeChange != rtc.getHour(true))
    {
        // We have passed an hour, update double temp log.
        tempMonitor.setTempLog(0, rtc.getHour(true), tempMonitor.getCurrentTemp());
        timeChange = rtc.getHour(true);
    }
    // Day has changed, reset RTC from NTP pool, move current tempLog to previous tempLog location and clear current tempLog.
    if (dayChange != rtc.getDayofWeek())
    {
        for (int i = 0; i < 24; i++)
        {
            tempMonitor.setTempLog(1, i, tempMonitor.getTempLog(0, i));
            tempMonitor.setTempLog(0, i, 0);
        }
        tempMonitor.setTempLog(0, 0, tempMonitor.getTempLog(1, 0));
        setRTC();
    }
    sys.setCurrentTime(rtc.getTime());
}
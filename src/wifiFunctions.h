#ifndef WIFI_H
#define WIFI_H
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WIFI
{
private:
    String wifiStatus;

public:
    WIFI() : wifiStatus("Not Set             "){};

    void setStatus(String status)
    {
        wifiStatus = status;
    }

    String getCurrentStatus()
    {
        return wifiStatus;
    }

    void webServer();
    void settingsConfig();
};

#endif
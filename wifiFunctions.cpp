#include "global.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
extern struct settings Settings;
extern TaskHandle_t doWifi;

void createWifiTask()
{
    xTaskCreate(
        doWiFi,
        "do Wifi",
        10000,
        NULL,
        0,
        &doWifi);
}

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

const char settings_index_html[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
    <title>Aquarium Controller Settings</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <head>
        <style>
            body {
                background-color: #1e81b0;
                color: white;
                font-family: "Helvetica Neue", Helvetica, sans-serif;
            }

            #container {
                width: 70%;
                height: 50%;
            }

            input[type=text] {
                margin: 3px;
            }

            input[type=submit] {
                margin-top: 20px;
                float: right;
            }
        </style>
    </head>
<body>
    <center>
        <h1>Use the form below to configure controller settings</h1>
        <form action="/get">
            <div id="container">
                <b>WiFi Settings:</b><br />
                <input type="hidden" name="submitted">
                SSID: <br /><input type="text" name="wifiSSID"><br />
                Password: <br /><input type="text" name="wifiPassword"><br /><br />
                <b>Equipment Names (Leave blank if not in use):</b></br>
                Device One:<br /> <input type="text" name="deviceOne"><br />
                Device Two:<br /> <input type="text" name="deviceTwo"><br />
                Device Three:<br /> <input type="text" name="deviceThree"><br />
                Device Four:<br /> <input type="text" name="deviceFour"><br />
                Device Five:<br /> <input type="text" name="deviceFive"><br />
                Device Six:<br /> <input type="text" name="deviceSix"><br />
                Device Seven:<br /> <input type="text" name="deviceSeven"><br />
                Device Eight:<br /> <input type="text" name="deviceEight"><br />
                <input type="submit" value="Submit">
        </form>
        </div>
    </center>
</body>
</html>)";

void doWiFi(void *parameter)
{
    for (;;)
    {
        if (Settings.wifiSsid != "")
        {
            // Disable AP connection if currently available.
            WiFi.softAPdisconnect(true);
            // Set WiFi mode to station mode.
            WiFi.mode(WIFI_STA);
            WiFi.begin(Settings.wifiSsid, Settings.wifiPassword);
            // While WiFi status is not connected
            while (WiFi.status() != WL_CONNECTED)
            {
                // Use switch case to allow us to create readable text based on WiFi status return.
                switch (WiFi.status())
                {
                case 0:
                    menuArray[0][1] = "IP: Network Init";
                    break;
                case 1:
                    menuArray[0][1] = "IP: No SSID Avail";
                    break;
                case 4:
                    menuArray[0][1] = "IP: Network Failed";
                    break;
                case 5:
                    menuArray[0][1] = "IP: Connection Lost";
                    break;
                case 6:
                    menuArray[0][1] = "IP: Disconnected";
                    break;
                }
                // Delay for 500ms between connection attempts.
                vTaskDelay(500);
            }
            // Connected - amened menuArray with the current IP address.
            menuArray[0][1] = "IP: " + String(WiFi.localIP().toString().c_str());
            // As a connection has been established, delete the associated task.
            vTaskDelete(doWifi);
        }
        else
        {
            // Settings SSID is blank and thus, no useable credentials exist.
            menuArray[0][1] = "IP: SSID Not Set";
        }
    }
}

void webServer()
{
    /*Webserver to do.
     */
}

void setWifi()
{
    selectableMenu = false;

    // Back button has been pressed, reconnect WiFi using the previously existing credentials (if any)
    // as we previously disconnected WiFi once this menu page was accessed.
    if (buttonBack == LOW)
    {
        createWifiTask();
    }

    // WiFi mode is set to Access Point, create webserver.
    if (WiFi.getMode() == WIFI_MODE_AP)
    {
        // Set menu to display connection details.
        menuArray[8][0] = "Connect to SSID:";
        menuArray[8][1] = "AquariumController";
        menuArray[8][2] = "Navigate to: ";
        menuArray[8][3] = String(WiFi.softAPIP().toString().c_str());
        // Push settings webpage to client.
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/html", settings_index_html); });
        // GET request
        server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
    //Get "submitted" param letting us know that the form has been submitted.
    if(request->hasParam("submitted")){
        Serial.println(request->getParam("wifiSSID")->value());
        Serial.println(request->getParam("wifiPassword")->value());
        Serial.println(request->getParam("deviceOne")->value());
        //Assign values to settings struct.
        Settings.wifiSsid = request->getParam("wifiSSID")->value();
        Settings.wifiPassword = request->getParam("wifiPassword")->value();
    }
    //Let user know that the settings have been saved, start a new WiFi connect task to connect to new credentials.
    //Return to init screen.
        request->send(200, "text/html", "<center>Settings Saved! <br />Returning to main screen.</center>"); 
                createWifiTask();
        currentMenu = 0; });

        server.onNotFound(notFound);
        server.begin();
    }
    else
    {
        // WiFi mode is not in AP, set to AP with the SSID "AquariumController".
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP("AquariumController");
    }
}
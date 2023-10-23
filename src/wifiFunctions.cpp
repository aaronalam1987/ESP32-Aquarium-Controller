#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "wifiFunctions.h"
#include "menuControl.h"
#include "espNow.h"

extern espNow espnow;
WIFI wifi;
AsyncWebServer web_server(80);
// wifiFunctions requires access to most of the previously declared class instances to provide the functionality for configuring settings through a webpage.
// Web access also provides current equipment status (on/off) as well as a temperature graph for the current 24 hours and previous 24 hours, providing an average 24 hour reading.
extern TempMonitor tempMonitor;
extern Menus menu;
extern Settings settings;
extern System sys;
extern TaskHandle_t doWifi;

// Pointer to our currently selected web page, this changes based on current mode (Network server / AP server).
const char *currWebPage{0};

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

// This is our settings index html code and is presented when the WiFi mode is set to AP (access point).
// It provides the functionality for configuring settings such as wifi SSID and password as well as device naming and GPIO configuration.
const char settings_index_html[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
    <title>Aquarium Controller Settings</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
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
                SSID: <br /><input type="text" name="wifiSSID" value="%WIFISSID%"><br />
                Password: <br /><input type="text" name="wifiPassword" value="%WIFIPW%"><br /><br />
                <b>Equipment Names:</b><br />
				<i>(Leave blank if not in use)</i><br /><br />
                Device One:<br /> <input type="text" name="deviceOne" value="%DEVICEONENAME%"><br />
                Device Two:<br /> <input type="text" name="deviceTwo" value="%DEVICETWONAME%" ><br />
                Device Three:<br /> <input type="text" name="deviceThree" value="%DEVICETHREENAME%" ><br />
                Device Four:<br /> <input type="text" name="deviceFour" value="%DEVICEFOURNAME%"><br />
                Device Five:<br /> <input type="text" name="deviceFive" value="%DEVICEFIVENAME%"><br />
                Device Six:<br /> <input type="text" name="deviceSix" value="%DEVICESIXNAME%"><br />
                Device Seven:<br /> <input type="text" name="deviceSeven" value="%DEVICESEVENNAME%"><br />
                Device Eight:<br /> <input type="text" name="deviceEight" value="%DEVICEEIGHTNAME%"><br /><br />
                <b>Slave Mac Address:</b><br />
                <i>Leave blank to broadcast to all ESP-NOW devices</i><br />
                <input type="text" name="sMac"><br /><br />
                Enable Automatic Temperature Control <input type="checkbox" name="enableTempCon" value="1" %TEMPCONCHECK%><br />
				Enable Automatic Light Control <input type="checkbox" name="enableLightCon" value="1" %LIGHTCONCHECK%><br />
				Enable Automatic ATO Control <input type="checkbox" name="enableATO" value="1" %ATOCHECK%><br /><br />
                <input type="submit" value="Submit">
        </form>
        </div>
    </center>
</body>
</html>)";

// This page is the controller homepage and provides the current status of equipment, current temperature and 24/48 hour temperature graphs.
const char webserver_index[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>

<head>
    <title>Aquarium Controller Homepage</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
    <style>
        body {
            background-color: #1e81b0;
            color: white;
            font-family: "Helvetica Neue", Helvetica, sans-serif;
        }
    </style>
</head>

<body>
    <center>
        <h1>Aquarium Monitor</h1><br />
        <h3>Equipment Status</h3>
        %EQUIPMENTSTATUS%
        <h3>Current Temperature: <br /><i>%CURRENTTEMP%</i></h3><br />
        <h3>Temperature over current 24 hours</h3>
        <h5><i>This resets daily at 00:00</i></h5>
        <canvas id="tempChart" style="width:400px;max-width:600px"></canvas>
        <script>
            Chart.defaults.global.defaultFontColor = 'white';
            const xValues = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23];
            const yValues = [%TEMPCHART%];
            new Chart("tempChart", {
                type: "line",
                data: {
                    labels: xValues,
                    datasets: [{
                        fill: true,
                        lineTension: 0,
                        backgroundColor: "#1b749e",
                        borderColor: "white",
                        color: "white",
                        data: yValues,
                        label: "Temperature over 24 hours",
                    }]
                },
                options: {
                    legend: {
                        display: true,
                    },
                    scales: {
                        yAxes: [{ ticks: { min: 0, max: 50 } }],
                    }
                }
            });
        </script>
        <h3>Temperature over previous 24 hours</h3>
        <canvas id="tempChartPrev" style="width:400px;max-width:600px"></canvas>
        <script>
            Chart.defaults.global.defaultFontColor = 'white';
            const xValuesPrev = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23];
            const yValuesPrev = [%TEMPCHARTPREV%];
            new Chart("tempChartPrev", {
                type: "line",
                data: {
                    labels: xValuesPrev,
                    datasets: [{
                        fill: true,
                        lineTension: 0,
                        backgroundColor: "#1b749e",
                        borderColor: "white",
                        color: "white",
                        data: yValuesPrev,
                        label: "Temperature over previous 24 hours",
                    }]
                },
                options: {
                    legend: {
                        display: true,
                    },
                    scales: {
                        yAxes: [{ ticks: { min: 0, max: 50 } }],
                    }
                }
            });
        </script>
        <h4>Average Temperature <i>(based off 24 hour reading)</i>:<br /><i>%AVGTEMP%</i></h4><br />
    </center>
</body>

</html>)";

// This long stretch replaces the placeholders in our raw literal html pages with variable values.
String processor(const String &var)
{
    if (var == "WIFISSID")
    {
        return String(settings.wifiSsid);
    }
    else if (var == "WIFIPW")
    {
        return String(settings.wifiPassword);
    }
    else if (var == "DEVICEONENAME")
    {
        return String(settings.deviceOneName);
    }
    else if (var == "DEVICETWONAME")
    {
        return String(settings.deviceTwoName);
    }
    else if (var == "DEVICETHREENAME")
    {
        return String(settings.deviceThreeName);
    }
    else if (var == "DEVICEFOURNAME")
    {
        return String(settings.deviceFourName);
    }
    else if (var == "DEVICEFIVENAME")
    {
        return String(settings.deviceFiveName);
    }
    else if (var == "DEVICESIXNAME")
    {
        return String(settings.deviceSixName);
    }
    else if (var == "DEVICESEVENNAME")
    {
        return String(settings.deviceSevenName);
    }
    else if (var == "DEVICEEIGHTNAME")
    {
        return String(settings.deviceEightName);
    }
    else if (var == "EQUIPMENTSTATUS")
    {
        return String(settings.deviceOneName + sys.getEquipmentStatus(0) + "<br />" + settings.deviceTwoName + sys.getEquipmentStatus(1) + "<br />" + settings.deviceThreeName + sys.getEquipmentStatus(2) + "<br />" + settings.deviceFourName + sys.getEquipmentStatus(3) + "<br />" + settings.deviceFiveName + sys.getEquipmentStatus(4) + "<br />" + settings.deviceSixName + sys.getEquipmentStatus(5) + "<br />" + settings.deviceSevenName + sys.getEquipmentStatus(6) + "<br />" + settings.deviceEightName + sys.getEquipmentStatus(7) + "<br />");
    }
    else if (var == "CURRENTTEMP")
    {
        return String(tempMonitor.getCurrentTemp());
    }
    else if (var == "TEMPCHART")
    {
        String tempChart;
        for (int i = 0; i < 24; i++)
        {
            if (i < 23)
            {
                tempChart += String(tempMonitor.getTempLog(0, i)) + ",";
            }
            else
            {
                tempChart += String(tempMonitor.getTempLog(0, i));
            }
        }
        return String(tempChart);
    }
    else if (var == "TEMPCHARTPREV")
    {
        String tempChart;
        for (int i = 0; i < 24; i++)
        {
            if (i < 23)
            {
                tempChart += String(tempMonitor.getTempLog(1, i)) + ",";
            }
            else
            {
                tempChart += String(tempMonitor.getTempLog(1, i));
            }
        }
        return String(tempChart);
    }
    else if (var == "AVGTEMP")
    {
        double avgTemp;
        for (int i = 0; i < 24; i++)
        {
            avgTemp += tempMonitor.getTempLog(1, i);
        }
        double avg = avgTemp / 24;
        return String(avg);
    }
    return String();
}

// doWiFI is responsible for connecting to the provided wifi network, once connected it updates the current time and provides network access to the homepage.
void doWiFi(void *parameter)
{
    for (;;)
    {
        // Disable AP connection if currently available.
        WiFi.softAPdisconnect(true);
        // Set WiFi mode to station mode.
        WiFi.mode(WIFI_STA);
        WiFi.setSleep(WIFI_PS_NONE);
        espnow.espNowInit();
        if (settings.wifiSsid != "")
        {
            // Connect to stored wifi credentials.
            WiFi.begin(settings.wifiSsid, settings.wifiPassword);
            // While WiFi status is not connected
            while (WiFi.status() != WL_CONNECTED)
            {
                // Use switch case to allow us to create readable text based on WiFi status return.
                switch (WiFi.status())
                {
                case 0:
                    wifi.setStatus("IP: Network Init");
                    break;
                case 1:
                    wifi.setStatus("IP: No SSID Avail");
                    break;
                case 4:
                    wifi.setStatus("IP: Network Failed");
                    break;
                case 5:
                    wifi.setStatus("IP: Connection Lost");
                    break;
                case 6:
                    wifi.setStatus("IP: Disconnected");
                    break;
                }
                // Delay for 500ms between connection attempts.
                vTaskDelay(500);
            }
            // Connected - amend menuArray with the current IP address.
            wifi.setStatus("IP: " + String(WiFi.localIP().toString().c_str()));
            // Assign currWebPage pointer to webserver index page.
            currWebPage = webserver_index;
            // Set RTC from ntp pool.
            setRTC();
            // As a connection has been established, delete the associated task.
            vTaskDelete(doWifi);
        }
        else
        {
            // Settings SSID is blank and thus, no useable credentials exist.
            wifi.setStatus("IP: SSID Not Set    ");
        }
    }
}

void WIFI::webServer()
{
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/html", currWebPage, processor); });

    // GET request
    web_server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                      // Get "submitted" param letting us know that the form has been submitted.
                      // Only the webpage for settings index has the parameter of "submitted".
                      if (request->hasParam("submitted"))
                      {
                          // Assign values to settings.
                          settings.wifiSsid = request->getParam("wifiSSID")->value();
                          settings.wifiPassword = request->getParam("wifiPassword")->value();
                          settings.deviceOneName = request->getParam("deviceOne")->value();
                          settings.deviceTwoName = request->getParam("deviceTwo")->value();
                          settings.deviceThreeName = request->getParam("deviceThree")->value();
                          settings.deviceFourName = request->getParam("deviceFour")->value();
                          settings.deviceFiveName = request->getParam("deviceFive")->value();
                          settings.deviceSixName = request->getParam("deviceSix")->value();
                          settings.deviceSevenName = request->getParam("deviceSeven")->value();
                          settings.deviceEightName = request->getParam("deviceEight")->value();
                          settings.enableTempCon = request->hasParam("enableTempCon") ? true : false;
                          settings.enableLightCon = request->hasParam("enableLightCon") ? true : false;
                          settings.enableATO = request->hasParam("enableATO") ? true : false;

                      }
                      // Let user know that the settings have been saved, start a new WiFi connect task to connect to new credentials.
                      // Return to init screen.
                      request->send(200, "text/html", "<center>Settings Saved! <br />Returning to main screen.</center>");
                      menu.setCurrentMenu(menuInit);
                      // Set currWebPage to webserver_index which is the controller homepage.
                      currWebPage = webserver_index;
                      delay(200);
                      createWifiTask();
                      // We need to save new settings to eeprom.
                      sys.setEepromWrite(true); });

    web_server.onNotFound(notFound);
    web_server.begin();
}

void WIFI::settingsConfig()
{
    // Menu is not selectable.
    menu.menuSelectable(false);

    // Back button has been pressed, reconnect WiFi using the previously existing credentials (if any)
    // as we previously disconnected WiFi once this menu page was accessed.
    if (sys.buttonBack == LOW)
    {
        createWifiTask();
    }

    // WiFi mode is set to Access Point, create webserver.
    if (WiFi.getMode() == WIFI_MODE_AP)
    {
        wifi.setStatus(WiFi.softAPIP().toString().c_str());
        // Clear menu.
        menu.clearMenu();

        // Build our menu.
        menu.setMenu(0, menuLineOne, "Connect to SSID:");
        menu.setMenu(0, menuLineTwo, "AquariumController");
        menu.setMenu(0, menuLineThree, "Navigate to:");
        menu.setMenu(0, menuLineFour, wifi.getCurrentStatus());
    }
    else
    {
        // WiFi mode is not in AP, set to AP with the SSID "AquariumController" and assign webpage pointer to settings page.
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP("AquariumController");
        currWebPage = settings_index_html;
    }
}
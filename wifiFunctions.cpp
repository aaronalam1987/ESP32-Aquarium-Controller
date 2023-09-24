#include "global.h"
#include "tempMonitor.h"
#include "settings.h"
#include "wifiFunctions.h"
#include "menuControl.h"

WIFI wifi;

AsyncWebServer web_server(80);
extern TempMonitor tempMonitor;
extern Menus menu;
extern Settings settings;
extern System sys;
extern TaskHandle_t doWifi;
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
                Device Two:<br /> <input type="text" name="deviceTwo" value="%DEVICETWONAME%"><br />
                Device Three:<br /> <input type="text" name="deviceThree" value="%DEVICETHREENAME%"><br />
                Device Four:<br /> <input type="text" name="deviceFour" value="%DEVICEFOURNAME%"><br />
                Device Five:<br /> <input type="text" name="deviceFive" value="%DEVICEFIVENAME%"><br />
                Device Six:<br /> <input type="text" name="deviceSix" value="%DEVICESIXNAME%"><br />
                Device Seven:<br /> <input type="text" name="deviceSeven" value="%DEVICESEVENNAME%"><br />
                Device Eight:<br /> <input type="text" name="deviceEight" value="%DEVICEEIGHTNAME%"><br /><br />
				Select Device Modes:<br />
				<i>Select which device is connected to lighting, heating, cooling and auto top off equipment</i><br /><br />
				Lighting:<br />
				<select id="lighting" name="lighting">
					<option value="0">Unused</option>
					<option value="27">Device One</option>
					<option value="26">Device Two</option>
					<option value="25">Device Three</option>
					<option value="33">Device Four</option>
					<option value="32">Device Five</option>
					<option value="17">Device Six</option>
					<option value="18">Device Seven</option>
					<option value="19">Device Eight</option>
				</select><br />
				Heating:<br />
				<select id="heating" name="heating">
					<option value="0">Unused</option>
					<option value="27">Device One</option>
					<option value="26">Device Two</option>
					<option value="25">Device Three</option>
					<option value="33">Device Four</option>
					<option value="32">Device Five</option>
					<option value="17">Device Six</option>
					<option value="18">Device Seven</option>
					<option value="19">Device Eight</option>
				</select><br />
				Cooling:<br />
				<select id="cooling" name="cooling">
					<option value="0">Unused</option>
					<option value="27">Device One</option>
					<option value="26">Device Two</option>
					<option value="25">Device Three</option>
					<option value="33">Device Four</option>
					<option value="32">Device Five</option>
					<option value="17">Device Six</option>
					<option value="18">Device Seven</option>
					<option value="19">Device Eight</option>
				</select><br />
				Auto Top-Off:<br />
				<select id="ato" name="ato">
					<option value="0">Unused</option>
					<option value="27">Device One</option>
					<option value="26">Device Two</option>
					<option value="25">Device Three</option>
					<option value="33">Device Four</option>
					<option value="32">Device Five</option>
					<option value="17">Device Six</option>
					<option value="18">Device Seven</option>
					<option value="19">Device Eight</option>
				</select><br />
                <input type="submit" value="Submit">
        </form>
        </div>
    </center>
</body>
</html>)";

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

String status(int status)
{
    // Converts the status int into a readable string, to be used in menus.
    // 1 represents the relay being "off" as connected devices are connected via "normally closed" to be safe in the event of some sort of system failure.
    String currStatus = "";
    if (status == 0)
    {
        currStatus = " Off";
    }
    else
    {
        currStatus = " On ";
    }
    return currStatus;
}

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
        return String(settings.deviceOneName + "<br />" + settings.deviceTwoName + "<br />" + settings.deviceThreeName + "<br />" + settings.deviceFourName + "<br />" + settings.deviceFiveName + "<br />" + settings.deviceSixName + "<br />" + settings.deviceSevenName + "<br />" + settings.deviceEightName + "<br />");
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
                // tempChart += String(tempLog[0][i]) + ",";
            }
            else
            {
                tempChart += String(tempMonitor.getTempLog(0, i));
                // tempChart += String(tempLog[0][i]);
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
                // tempChart += String(tempLog[1][i]) + ",";
            }
            else
            {
                tempChart += String(tempMonitor.getTempLog(1, i));
                // tempChart += String(tempLog[1][i]);
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
            // avgTemp += tempLog[1][i];
        }
        double avg = avgTemp / 24;
        return String(avg);
    }
    return String();
}

void doWiFi(void *parameter)
{
    for (;;)
    {
        if (settings.wifiSsid != "")
        {
            // Disable AP connection if currently available.
            WiFi.softAPdisconnect(true);
            // Set WiFi mode to station mode.
            WiFi.mode(WIFI_STA);
            WiFi.begin(settings.wifiSsid, settings.wifiPassword);
            // While WiFi status is not connected
            while (WiFi.status() != WL_CONNECTED)
            {
                // Use switch case to allow us to create readable text based on WiFi status return.
                switch (WiFi.status())
                {
                case 0:
                    wifi.setStatus("IP: Network Init    ");
                    break;
                case 1:
                    wifi.setStatus("IP: No SSID Avail   ");
                    break;
                case 4:
                    wifi.setStatus("IP: Network Failed  ");
                    break;
                case 5:
                    wifi.setStatus("IP: Connection Lost ");
                    break;
                case 6:
                    wifi.setStatus("IP: Disconnected    ");
                    break;
                }
                // Delay for 500ms between connection attempts.
                vTaskDelay(500);
            }
            // Connected - amend menuArray with the current IP address.
            wifi.setStatus("IP: " + String(WiFi.localIP().toString().c_str()));
            // Assign currWebPage pointer to webserver index page.
            currWebPage = webserver_index;
            // Initialise webServer.
            // Set RTC from ntp pool.
            setRTC();
            // As a connection has been established, delete the associated task.
            vTaskDelete(doWifi);
        }
        else
        {
            // Settings SSID is blank and thus, no useable credentials exist.
            // menuArray[0][1] = "IP: SSID Not Set";
            wifi.setStatus("IP: SSID Not Set    ");
        }
    }
}

void webServer()
{
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/html", currWebPage, processor); });

    // GET request
    web_server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                      // Get "submitted" param letting us know that the form has been submitted.
                      if (request->hasParam("submitted"))
                      {
                          // Assign values to settings struct.
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
                          settings.lightGPIO = request->getParam("lighting")->value().toInt();
                          settings.heatingGPIO = request->getParam("heating")->value().toInt();
                          settings.coolingGPIO = request->getParam("cooling")->value().toInt();
                          settings.atoGPIO = request->getParam("ato")->value().toInt();
                      }
                      // Let user know that the settings have been saved, start a new WiFi connect task to connect to new credentials.
                      // Return to init screen.
                      request->send(200, "text/html", "<center>Settings Saved! <br />Returning to main screen.</center>");
                      menu.setCurrentMenu(menuInit);
                      currWebPage = webserver_index;
                      delay(200);
                      createWifiTask();
                      sys.setEepromWrite(true); });

    web_server.onNotFound(notFound);
    web_server.begin();
}

void settingsConfig()
{
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
        menu.clearMenu();
        menu.setMenu(0, menuLineOne, "Connect to SSID:    ");
        menu.setMenu(0, menuLineTwo, "AquariumController  ");
        menu.setMenu(0, menuLineThree, "Navigate to:      ");
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
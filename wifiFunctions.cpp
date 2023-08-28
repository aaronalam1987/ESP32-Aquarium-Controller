#include "global.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
extern struct settings Settings;
extern TaskHandle_t doWifi;
extern struct connectedEquipment deviceOne,
    deviceTwo, deviceThree, deviceFour, deviceFive, deviceSix, deviceSeven, deviceEight;

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
        return String(Settings.wifiSsid);
    }
    else if (var == "WIFIPW")
    {
        return String(Settings.wifiPassword);
    }
    else if (var == "DEVICEONENAME")
    {
        return String(Settings.deviceOneName);
    }
    else if (var == "DEVICETWONAME")
    {
        return String(Settings.deviceTwoName);
    }
    else if (var == "DEVICETHREENAME")
    {
        return String(Settings.deviceThreeName);
    }
    else if (var == "DEVICEFOURNAME")
    {
        return String(Settings.deviceFourName);
    }
    else if (var == "DEVICEFIVENAME")
    {
        return String(Settings.deviceFiveName);
    }
    else if (var == "DEVICESIXNAME")
    {
        return String(Settings.deviceSixName);
    }
    else if (var == "DEVICESEVENNAME")
    {
        return String(Settings.deviceSevenName);
    }
    else if (var == "DEVICEEIGHTNAME")
    {
        return String(Settings.deviceEightName);
    }
    else if (var == "EQUIPMENTSTATUS")
    {
        return String(deviceOne.name + ":" + status(deviceOne.status) + "<br />" + deviceTwo.name + ":" + status(deviceTwo.status) + "<br />" + deviceThree.name + ":" + status(deviceThree.status) + "<br />" + deviceFour.name + ":" + status(deviceFour.status) + "<br />" + deviceFive.name + ":" + status(deviceFive.status) + "<br />" + deviceSix.name + ":" + status(deviceSix.status) + "<br />" + deviceSeven.name + ":" + status(deviceSeven.status) + "<br />" + deviceEight.name + ":" + status(deviceEight.status) + "<br />");
    }
    else if (var == "CURRENTTEMP")
    {
        return String(currentTemp);
    }
    else if (var == "TEMPCHART")
    {
        String tempChart;
        for (int i = 0; i < 24; i++)
        {
            if (i < 23)
            {
                tempChart += String(tempLog[0][i]) + ",";
            }
            else
            {
                tempChart += String(tempLog[0][i]);
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
                tempChart += String(tempLog[1][i]) + ",";
            }
            else
            {
                tempChart += String(tempLog[1][i]);
            }
        }
        return String(tempChart);
    }
    else if (var == "AVGTEMP")
    {
        double avgTemp;
        for (int i = 0; i < 24; i++)
        {
            avgTemp += tempLog[1][i];
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
            // Connected - amend menuArray with the current IP address.
            menuArray[0][1] = "IP: " + String(WiFi.localIP().toString().c_str());
            webServer();
            // Set RTC from ntp pool.
            setRTC();
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
    if (WiFi.status() != WL_DISCONNECTED)
    {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/html", webserver_index, processor); });
        server.onNotFound(notFound);
        server.begin();
    }
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
                  { request->send_P(200, "text/html", settings_index_html, processor); });
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
        Settings.deviceOneName = request->getParam("deviceOne")->value();
        Settings.deviceTwoName = request->getParam("deviceTwo")->value();
        Settings.deviceThreeName = request->getParam("deviceThree")->value();
        Settings.deviceFourName = request->getParam("deviceFour")->value();
        Settings.deviceFiveName = request->getParam("deviceFive")->value();
        Settings.deviceSixName = request->getParam("deviceSix")->value();
        Settings.deviceSevenName = request->getParam("deviceSeven")->value();
        Settings.deviceEightName = request->getParam("deviceEight")->value();
        Settings.lightGPIO = request->getParam("lighting")->value().toInt();
        Settings.heatingGPIO = request->getParam("heating")->value().toInt();
        Settings.coolingGPIO = request->getParam("cooling")->value().toInt();
        Settings.atoGPIO = request->getParam("ato")->value().toInt();
    }
    //Let user know that the settings have been saved, start a new WiFi connect task to connect to new credentials.
    //Return to init screen.
        request->send(200, "text/html", "<center>Settings Saved! <br />Returning to main screen.</center>"); 
                createWifiTask();
                delay(200);
                eepromWrite = true;
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
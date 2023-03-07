#include "captivePortal.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <FS.h>
#include "SPIFFS.h"
#define FILE_PATH "/../../config/Config_File.json"
// #include <ESPAsync_WiFiManager.h>
#include <ESPAsyncWiFiManager.h>
#include<ArduinoJson.h>



// CaptivePortal::CaptivePortal():server(80) {
//     loginStartTime = millis();
//     isTimedOut = false;
//     AsyncWiFiManager wiFiManager(&server,&dns);
// }

CaptivePortal::CaptivePortal(AsyncWebServer& server, DNSServer& dns, AsyncWiFiManager& wifiManager) 
    : server(server), dns(dns), wifiManager(wifiManager) {
    // loginStartTime = millis();
    // isTimedOut = false;
    }


void CaptivePortal::init() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        if (!isTimedOut) {
            String html = "<html><head><style>body{display: flex;flex-direction: column;align-items: center;background-color: lightgray;}img{width: 150px;height: 150px;margin-top: 50px;}form{background-color: white;padding: 50px;box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2);border-radius: 10px;}input[type='password']{width: 100%;padding: 12px 20px;margin: 8px 0;box-sizing: border-box;border: 2px solid lightgray;border-radius: 4px;}input[type='submit']{width: 100%;background-color: green;color: white;padding: 14px 20px;margin: 8px 0;border: none;border-radius: 4px;cursor: pointer;}input[type='submit']:hover{background-color: darkgreen;}</style></head><body><img src='data:image/png;base64,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'><form action='/login' method='post'><input type='password' name='password'/><input type='submit' value='Submit'/></form></body></html>";
            request->send(200, "text/html",html);
        } else {
            handleTimeout();
        }
    });
    server.on("/login", HTTP_POST, [this](AsyncWebServerRequest *request){
        if (request->hasParam("password", true) && checkPassword(request->getParam("password", true)->value())) {
            loginStartTime = millis();
            isTimedOut = false;
            isAuthenticated = true;
            request->redirect("/menu");
        } else {
            request->send(401, "text/plain", "Unauthorized");
        }
    });
    server.on("/menu", HTTP_GET, [](AsyncWebServerRequest *request){
        String html = "<html><head><style>body{text-align:center;} .button{display:block;margin:50px auto;padding:10px 20px;font-size:20px;background-color:#45a049;color:white;border-radius:5px;text-decoration:none;}</style></head><body><a class='button' href='/wifi_config'>WiFi Configuration</a><br/><a class='button' href='/main_config'>Main Configuration</a><br/><a class='button' href='/soft_reboot'>Soft Reboot</a></body></html>";
        request->send(200, "text/html", html);
    });

//     server.on("/wifi_config", HTTP_GET, [](AsyncWebServerRequest *request)
// {
//      String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>";
//   html += "body {background-color: #f2f2f2; font-family: Arial, sans-serif;}";
//   html += "h2 {text-align: center;}";
//   html += "table {border-collapse: collapse; width: 100%;}";
//   html += "th, td {text-align: left; padding: 8px;}";
//   html += "th {background-color: #4CAF50; color: white;}";
//   html += "tr:nth-child(even) {background-color: #f2f2f2;}";
//   html += "form {max-width: 500px; margin: auto;}";
//   html += "input[type='text'], input[type='password'], select {width: 100%; padding: 12px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; margin-top: 6px; margin-bottom: 16px;}";
//   html += "input[type='submit'] {background-color: #4CAF50; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer;}";
//   html += "input[type='submit']:hover {background-color: #45a049;}";
//   html += "@media screen and (max-width: 600px) {";
//   html += "input[type='text'], input[type='password'], select {width: 100%;}";
//   html += "}";
//   html += "</style></head><body>";
//   html += "<h2>Wi-Fi Configuration</h2>";
//   html += "<h2>Wi-Fi Configuration</h2>";

//   // Get the list of available access points
//   int numNetworks = WiFi.scanNetworks();
//   if (numNetworks == 0) {
//     html += "<p>No access points found</p>";
//   }
//   else {
//     // Sort the access points by signal strength
//     int indices[numNetworks];
//     for (int i = 0; i < numNetworks; i++) {
//       indices[i] = i;
//     }
//     for (int i = 0; i < numNetworks; i++) {
//       for (int j = i + 1; j < numNetworks; j++) {
//         if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
//           std::swap(indices[i], indices[j]);
//         }
//       }
//     }

//     // Add a table with the list of access points
//     html += "<table><thead><tr><th>SSID</th><th>Signal Strength</th></tr></thead><tbody>";
//     for (int i = 0; i < numNetworks; i++) {
//       int index = indices[i];
//       int rssi = WiFi.RSSI(index);

//       // Calculate signal strength in percentage
//       int signalPercentage = 0;
//       if (rssi <= -100) {
//         signalPercentage = 0;
//       }
//       else if (rssi >= -50) {
//         signalPercentage = 100;
//       }
//       else {
//         signalPercentage = 2 * (rssi + 100);
//       }

//       // Add Wi-Fi signal icon based on signal strength
//       String signalIcon = "<i class='fas fa-wifi' style='color: green;'></i>";
//       if (signalPercentage <= 20) {
//         signalIcon = "<i class='fas fa-wifi' style='color: red;'></i>";
//       }
//       else if (signalPercentage <= 60) {
//         signalIcon = "<i class='fas fa-wifi' style='color: orange;'></i>";
//       }
//       else if (signalPercentage <= 80) {
//         signalIcon = "<i class='fas fa-wifi' style='color: yellow;'></i>";
//       }

//       html += "<tr><td>" + WiFi.SSID(index) + "</td><td>" + signalIcon + "</td></tr>";
//     }
//     html += "</tbody></table>";

//     // Add a form for entering the Wi-Fi credentials
//     html += "<form method='POST' action='/wifi_save_config'>";
//     html += "<p><label for='ssid'>SSID:</label><br><select id='ssid' name='ssid'>";
//     for (int i = 0; i < numNetworks; i++) {
//         int index = indices[i];
//         html += "<option value='" + WiFi.SSID(index) + "'>" + WiFi.SSID(index) + "</option>";
//     }
//     html += "</select></p><p><label>Password:</label><br><input type='password' name='password'></p>";
//     html += "<p><input type='submit' value='Save'></p></form>";
//   }
//   html += "</body></html>";
//   request->send(200, "text/html", html); 
// });


   

    server.on("/wifi_config", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>";
  html += "body {background-color: #f2f2f2; font-family: Arial, sans-serif;}";
  html += "h2 {text-align: center;}";
  html += "table {border-collapse: collapse; width: 100%;}";
  html += "th, td {text-align: left; padding: 8px;}";
  html += "th {background-color: #4CAF50; color: white;}";
  html += "tr:nth-child(even) {background-color: #f2f2f2;}";
  html += "form {max-width: 500px; margin: auto;}";
  html += "input[type='text'], input[type='password'], select {width: 100%; padding: 12px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; margin-top: 6px; margin-bottom: 16px;}";
  html += "input[type='submit'] {background-color: #4CAF50; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer;}";
  html += "input[type='submit']:hover {background-color: #45a049;}";
  html += "@media screen and (max-width: 600px) {";
  html += "input[type='text'], input[type='password'], select {width: 100%;}";
  html += "}";
  html += "</style></head><body>";
  html += "<h2>Wi-Fi Configuration</h2>";

  // Get the list of available access points
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    html += "<p>No access points found</p>";
  }
  else {
    // Sort the access points by signal strength
    int indices[numNetworks];
    for (int i = 0; i < numNetworks; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < numNetworks; i++) {
      for (int j = i + 1; j < numNetworks; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }

    

    // Add a table with the list of access points
    html += "<table><thead><tr><th>SSID</th><th>RSSI</th></tr></thead><tbody>";
    for (int i = 0; i < numNetworks; i++) {
      int index = indices[i];
    //   html += "<tr><td>" + WiFi.SSID(index) + "</td><td>" + getSignalIcon(WiFi.RSSI(index)) + "</td></tr>";
    
      html += "<tr><td>" + WiFi.SSID(index) + "</td><td>" + String(WiFi.RSSI(index)) + " dBm</td></tr>";
    }
    html += "</tbody></table>";

    // Add a form for entering the Wi-Fi credentials
    html += "<form method='POST' action='/wifi_save_config'>";
    html += "<p><label for='ssid'>SSID:</label><br><select id='ssid' name='ssid'>";
    for (int i = 0; i < numNetworks; i++) {
        int index = indices[i];
        html += "<option value='" + WiFi.SSID(index) + "'>" + WiFi.SSID(index) + "</option>";
    }
    html += "</select></p><p><label>Password:</label><br><input type='password' name='password'></p>";
    html += "<p><input type='submit' value='Save'></p></form>";
  }
  request->send(200, "text/html", html); 
});



    

    // Register the "/wifi_save_config" route for handling the form submission
    server.on("/wifi_save_config", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String ssid = request->arg("ssid");
                  String password = request->arg("password");
                  AsyncWebServer server(80);
                  DNSServer dns;
                  AsyncWiFiManager wifiManager(&server, &dns);
                  wifiManager.setConfigPortalTimeout(60);
                  Serial.println("Test");
                  wifiManager.autoConnect(ssid.c_str(), password.c_str());

                  // Save the Wi-Fi credentials to SPIFFS
                  File file = SPIFFS.open("/wifi.txt", "w");
                  if (file)
                  {
                      file.println(ssid);
                      file.println(password);
                      file.close();
                      request->send(200, "text/plain", "Wi-Fi credentials saved");
                  }
                  else
                  {
                      request->send(500, "text/plain", "Failed to save Wi-Fi credentials");
                  }
              });


    server.on("/main_config", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Read the config.json file
        File configFile = SPIFFS.open("/Config_File.json", "r");
        if (!configFile) {
            request->send(500, "text/plain", "Failed to open config.json");
            return;
        }
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        // char buf[size];
        configFile.readBytes(buf.get(), size);
        configFile.close();

        // Parse the JSON data
        DynamicJsonDocument doc(JSON_SIZE);
        DeserializationError error = deserializeJson(doc, buf.get());
        if (error) {
            request->send(500, "text/plain", "Failed to parse config.json");
            return;
        }

        // Update the Wi-Fi credentials
        String ssid = request->arg("ssid");
        String password = request->arg("password");
        doc["WIFI_SSID"] = ssid;
        doc["WIFI_PSWD"] = password;

        // Save the updated config.json file
        File configFile2 = SPIFFS.open("/Config_File.json", "w");
        if (!configFile2) {
            request->send(500, "text/plain", "Failed to open config.json for writing");
            return;
        }
        serializeJson(doc, configFile2);
        configFile2.close();

        // Send the JSON response
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    server.on("/soft_reboot", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<html><body>Rebooting...</body></html>");
        delay(1000);
        ESP.restart();
    });



    server.begin();

}


    
    


void CaptivePortal::handle() {
    if (!isAuthenticated && millis() - loginStartTime > 300000) {
        isTimedOut = true;
        handleTimeout();
    }
}


void CaptivePortal::handleTimeout() {
    startAP();
    loginStartTime = millis();
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = "<html><body>Time exceeded</body></html>";
        request->send(200, "text/html", html);
    });
    server.begin();
}

bool CaptivePortal::checkPassword(String password) {
    return password == defaultPassword;
}

// String CaptivePortal::getSignalIcon(int rssi){
//     if (rssi >= -50) {
//         return "<i class='fas fa-wifi' style='color:#4CAF50;'></i><i class='fas fa-wifi' style='color:#4CAF50;'></i><i class='fas fa-wifi' style='color:#4CAF50;'></i>";
//     } 
//     else if (rssi >= -70) {
//         return "<i class='fas fa-wifi' style='color:#4CAF50;'></i><i class='fas fa-wifi' style='color:#4CAF50;'></i><i class='far fa-wifi' style='color:#4CAF50;'></i>";
//     }  
//     else if (rssi >= -80) {
//         return "<i class='fas fa-wifi' style='color:#4CAF50;'></i><i class='far fa-wifi' style='color:#4CAF50;'></i><i class='far fa-wifi' style='color:#4CAF50;'></i>";
//     } 
//     else {
//         return "<i class='far fa-wifi' style='color:#4CAF50;'></i><i class='far fa-wifi' style='color:#4CAF50;'></i><i class='far fa-wifi' style='color:#4CAF50;'></i>";
//     }

// }


#include <WiFi.h>
#include "webConfigManager.h"
#include "captivePortal.h"
#include <DNSServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncWebServer.h>
// #include <ESPAsync_WiFiManager_Lite.h>
// #define SCAN_WIFI_NETWORKS                  true
#include <esp_task_wdt.h>

#include<SPIFFS.h>

WebConfigManager configManager;
DNSServer dns;
AsyncWebServer server(80);
AsyncWiFiManager wiFiManager(&server,&dns);

CaptivePortal captivePortal(server,dns,wiFiManager);

void setup() {
    
    Serial.begin(115200);

    configManager.ssid_key_gen();

    configManager.startAP();

    configManager.checkHeap();

    captivePortal.init();

    esp_task_wdt_init(60, true);

    SPIFFS.format();

    SPIFFS.begin();
}

void loop() {
    configManager.checkClient();
    delay(5000);
    captivePortal.handle();
}

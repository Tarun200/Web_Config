

#include <WiFi.h>
#include "webConfigManager.h"
#include "captivePortal.h"
#include <DNSServer.h>

#include <ESPAsyncWebServer.h>

#include <esp_task_wdt.h>

#include<SPIFFS.h>

#include "webConfigDetection.h"


WebConfigManager configManager;

CaptivePortal captivePortal;



void setup() {
    
    Serial.begin(115200);



    configManager.ssid_key_gen();

    configManager.startAP();

    configManager.checkHeap();

    captivePortal.init();

    esp_task_wdt_init(60, true);
    if(!SPIFFS.begin()){
        Serial.println("SPIFFS initialization failed");
        return;
    }
    Serial.println("SPIFFS initialization successful");

    

}

void loop() {
    configManager.checkClient();
    delay(5000);
    captivePortal.handle();
}

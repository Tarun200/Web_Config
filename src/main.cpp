#include <WiFi.h>
#include "webConfigManager.h"
#include "captivePortal.h"
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <esp_task_wdt.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <RTClib.h>
#include <SPI.h>
#include "webConfigDetection.h"


// Define the name of the NVS partition to use
#define NVS_PARTITION_NAME "WebConfig"

WebConfigManager configManager;
CaptivePortal captivePortal;
//RTC_DS1307 rtc;
WebConfigDetection configDetection(NVS_PARTITION_NAME);


void setup() {
  

  Serial.begin(115200);
  // delay(5000);

  

  // if (!rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  // }

  // if (!rtc.isrunning()) {
  //   Serial.println("RTC is NOT running, let's set the time!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }

  // Check if the time difference is within the threshold value
  if (configDetection.isTimeWithinThreshold()) {
    configManager.ssid_key_gen();
    configManager.startAP();
    //configManager.checkHeap();
    captivePortal.init();
  } else {
    Serial.println("Time limit exceeded");
  }

  esp_task_wdt_init(90, true);
  if (!SPIFFS.begin()) {
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

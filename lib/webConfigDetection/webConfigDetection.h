#ifndef WEB_CONFIG_MODE_DETECTION_H
#define WEB_CONFIG_MODE_DETECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

class WebConfigModeDetection {
  public:
    WebConfigModeDetection();
    bool isWebConfigMode();
  private:
    Preferences preferences;
};
#endif



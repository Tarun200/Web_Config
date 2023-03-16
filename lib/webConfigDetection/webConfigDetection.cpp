#include "WebConfigDetection.h"




WebConfigModeDetection::WebConfigModeDetection() {
  preferences.begin("webconfig", false);

}

bool WebConfigModeDetection::isWebConfigMode() {
  unsigned long storedTimestamp = preferences.getULong("timestamp", 0);
  unsigned long currentTimestamp = millis() / 1000;
  bool isMode = (currentTimestamp - storedTimestamp) <= 10;
  preferences.putULong("timestamp", currentTimestamp);
  return isMode;
}





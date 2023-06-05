#ifndef _WEBCONFIG_DETECTION_H_
#define _WEBCONFIG_DETECTION_H_

#include <Preferences.h>
#include <RTClib.h>
#include<nvs_flash.h>
#include<Wire.h>

class WebConfigDetection {
  private:
    Preferences preferences;
    RTC_DS1307 rtc;
    const char* nvs_partition_name;
    const uint32_t time_diff_threshold = 20;
    
    
  public:
    WebConfigDetection(const char* nvs_partition_name);
    ~WebConfigDetection();
    bool isTimeWithinThreshold();
    void RTC_init();
};

#endif

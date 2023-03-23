#include "webConfigDetection.h"

WebConfigDetection::WebConfigDetection(const char* nvs_partition_name) {
  this->nvs_partition_name = nvs_partition_name;
  esp_err_t err = nvs_flash_init();
  if (err != ESP_OK) {
    Serial.println("Error initializing NVS");
    return;
  }
  preferences.begin(nvs_partition_name, false);
  //rtc.begin();
}

void WebConfigDetection::RTC_init() // Initialising the RTC
{
  Wire.begin();
  rtc.begin();

  if (!rtc.isrunning())
  {  
    Serial.println(F("\n RTC : ERROR"));
    //ERC = Bit_insert(ERC, 1, 1); // Inserted to Pos 1 
    //RTC_stat=2;
  }

  else
  {
    Serial.println(F("\n RTC : OK"));
    //RTC_stat=1;
  }
}

WebConfigDetection::~WebConfigDetection() {
  preferences.end();
}

bool WebConfigDetection::isTimeWithinThreshold() {
  RTC_init();
  DateTime storedDateTime(preferences.getUInt("timestamp", 0));
  Serial.print("Stored Timestamp: ");
  Serial.print(storedDateTime.year());
  Serial.print("-");
  Serial.print(storedDateTime.month());
  Serial.print("-");
  Serial.print(storedDateTime.day());
  Serial.print(" ");
  Serial.print(storedDateTime.hour());
  Serial.print(":");
  Serial.print(storedDateTime.minute());
  Serial.print(":");
  Serial.println(storedDateTime.second());

  DateTime currentDateTime = rtc.now();
  Serial.print("Current Timestamp: ");
  Serial.print(currentDateTime.year());
  Serial.print("-");
  Serial.print(currentDateTime.month());
  Serial.print("-");
  Serial.print(currentDateTime.day());
  Serial.print(" ");
  Serial.print(currentDateTime.hour());
  Serial.print(":");
  Serial.print(currentDateTime.minute());
  Serial.print(":");
  Serial.println(currentDateTime.second());
  uint32_t timeDiff = currentDateTime.unixtime() - storedDateTime.unixtime();
  Serial.println(timeDiff);

  if (timeDiff <= time_diff_threshold) {
    return true;
  }
  preferences.putUInt("timestamp", currentDateTime.unixtime());
  return false;
}

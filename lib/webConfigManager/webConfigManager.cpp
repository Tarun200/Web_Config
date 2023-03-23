#include <WiFi.h>
#include "webConfigManager.h"



#include "captivePortal.h"
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPAsyncWiFiManager.h>

AsyncWebServer server(80);
DNSServer dns;
// AsyncWiFiManager wifiManager(&server,&dns);

// CaptivePortal captivePortal(server,dns,wifiManager);
void WebConfigManager::ssid_key_gen()
{
    randomSeed(micros());
    char ssid_temp[] = "TMGY GWAY";
    int ssid_len = strlen(ssid_temp);
    if (ssid_len + 2 > sizeof(ssid)) {
        // handle the error, for example by using a default ssid
        strcpy(ssid, "TMGY GWAY");
    } else {
        strcpy(ssid, ssid_temp);
        ssid[ssid_len] = '0' + random(10);
        ssid[ssid_len+1] = '0' + random(10);
    }
    strcpy(password,"12345678");
    WiFi.softAP(ssid, password);
}

void WebConfigManager::startAP() {
	    WiFi.mode(WIFI_AP_STA);
        CaptivePortal captive;
        captive.init();
        }
	
	void WebConfigManager::checkClient() {
	    if (WiFi.softAPgetStationNum() > 0) {
	        forwardToCaptivePortal();
	    } else {
	        delay(timeout * 1000);
	        if (WiFi.softAPgetStationNum() == 0) {
	            exitWebConfig();
	        }
	    }
	}
	
	void WebConfigManager::forwardToCaptivePortal() {
    server.begin();
    if (!clientConnected) {
        WiFiClient client = server.available();
        if (client) {
            clientConnected = true;
        
            // handle the client connection
            // read data from the client
            // Send data to the client
            // Close the connection when done

            

            client.stop();
            clientConnected = false;
        }
    }
}

	
	void WebConfigManager::exitWebConfig() {
	    // WiFi.softAPdisconnect(true);
	    // delay(1000);
	}

    void WebConfigManager::checkHeap(){
        uint32_t freeHeap = esp_get_free_heap_size();
        Serial.println("Free heap: " + String(freeHeap));
    }

    







    
    

    

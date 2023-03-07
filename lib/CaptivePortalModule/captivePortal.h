#ifndef CaptivePortal_H
#define CaptivePortal_H
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "webConfigManager.h"
#include <DNSServer.h>
#include <ESPAsyncWiFiManager.h>
#define JSON_SIZE 1024



// #include <ESPAsync_WiFiManager_Lite.h>
// #define SCAN_WIFI_NETWORKS                  true


class CaptivePortal : public WebConfigManager{
    public:
        // CaptivePortal();
        CaptivePortal(AsyncWebServer& server, DNSServer& dns, AsyncWiFiManager& wifiManager);
        void init();
        void handle();
        unsigned long loginStartTime;
        bool isTimedOut;
        bool isAuthenticated;
        // AsyncWebServer server;
        // DNSServer dns;
        String getSignalIcon(int rssi);

    private:
        void handleTimeout();
        bool checkPassword(String password);
        const char* defaultPassword = "12345";
        AsyncWebServer& server;
        DNSServer& dns;
        AsyncWiFiManager& wifiManager;
       
};
#endif

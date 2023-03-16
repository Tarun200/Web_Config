#ifndef CaptivePortal_H
#define CaptivePortal_H
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "webConfigManager.h"
#include <DNSServer.h>



class CaptivePortal : public WebConfigManager{
    public:
        CaptivePortal();
        // CaptivePortal(AsyncWebServer& server, DNSServer& dns, AsyncWiFiManager& wifiManager);
        void init();
        void handle();
        unsigned long loginStartTime;
        bool isTimedOut;
        bool isAuthenticated;
        AsyncWebServer server;
        // DNSServer dns;
        

    private:
        void handleTimeout();
        bool checkPassword(String password);
        const char* defaultPassword = "12345";
        
        // DNSServer& dns;
        // AsyncWiFiManager& wifiManager;
       
};
#endif

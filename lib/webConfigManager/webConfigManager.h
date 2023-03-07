#ifndef WEBCONFIGMANAGER_H
#define WEBCONFIGMANAGER_H

#define SSID_LENGTH 20
#define PASSWORD_LENGTH 20
#define DEFAULT_TIMEOUT 30

class WebConfigManager {
private:
    char ssid[SSID_LENGTH];
    char password[PASSWORD_LENGTH];
    uint16_t timeout;
    WiFiServer server;
    uint16_t clientCount;
    bool clientConnected;

public:
    WebConfigManager():timeout(DEFAULT_TIMEOUT), clientCount(0), clientConnected(false) {};
    void ssid_key_gen();
    void startAP();
    void checkClient();
    void forwardToCaptivePortal();
    void exitWebConfig();
    void checkHeap();
};



#endif

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "Config.h"

enum WiFiStatus {
    WIFI_IDLE,
    WIFI_AP_MODE,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_FAILED
};

class SlapWiFiManager {
private:
    ConfigManager* configMgr;
    WiFiStatus status;
    unsigned long connectStartTime;
    static constexpr unsigned long CONNECT_TIMEOUT = 20000; // 20 seconds
    
public:
    SlapWiFiManager(ConfigManager* cfg) : configMgr(cfg), status(WIFI_IDLE), connectStartTime(0) {}
    
    // Initialize WiFi based on configuration
    void begin() {
        WiFi.mode(WIFI_MODE_NULL);
        delay(100);
        
        if (configMgr->isAPMode() || strlen(configMgr->getSSID()) == 0) {
            startAP();
        } else {
            startClient();
        }
    }
    
    // Start Access Point mode
    void startAP() {
        Serial.println("Starting AP mode...");
        
        WiFi.mode(WIFI_AP);
        
        // Configure custom IP: 10.0.0.1
        IPAddress local_ip(10, 0, 0, 1);
        IPAddress gateway(10, 0, 0, 1);
        IPAddress subnet(255, 255, 255, 0);
        
        if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
            Serial.println("Failed to configure AP IP");
        }
        
        // Start AP with no password (open network)
        bool success = WiFi.softAP("slap-ai");
        
        if (success) {
            status = WIFI_AP_MODE;
            Serial.println("AP started successfully");
            Serial.print("AP IP: ");
            Serial.println(WiFi.softAPIP());
            Serial.println("SSID: slap-ai (open)");
        } else {
            Serial.println("Failed to start AP");
            status = WIFI_FAILED;
        }
    }
    
    // Start Client mode (connect to WiFi)
    void startClient() {
        Serial.println("Starting Client mode...");
        Serial.printf("Connecting to: %s\n", configMgr->getSSID());
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(configMgr->getSSID(), configMgr->getPassword());
        
        status = WIFI_CONNECTING;
        connectStartTime = millis();
    }
    
    // Update WiFi connection status (call in loop)
    void update() {
        if (status == WIFI_CONNECTING) {
            if (WiFi.status() == WL_CONNECTED) {
                status = WIFI_CONNECTED;
                Serial.println("WiFi connected!");
                Serial.print("IP: ");
                Serial.println(WiFi.localIP());
                Serial.print("Signal: ");
                Serial.print(WiFi.RSSI());
                Serial.println(" dBm");
                
                // Start mDNS responder
                if (MDNS.begin("slap-ai")) {
                    Serial.println("mDNS responder started");
                    Serial.println("Access at: http://slap-ai.local");
                    
                    // Add service to mDNS-SD
                    MDNS.addService("http", "tcp", 80);
                } else {
                    Serial.println("Error starting mDNS");
                }
            } else if (millis() - connectStartTime > CONNECT_TIMEOUT) {
                // Connection timeout - revert to AP
                Serial.println("WiFi connection timeout - reverting to AP mode");
                WiFi.disconnect();
                configMgr->setAPMode(true);
                configMgr->save();
                startAP();
            }
        }
    }
    
    // Switch to AP mode
    void switchToAP() {
        WiFi.disconnect();
        delay(100);
        configMgr->setAPMode(true);
        configMgr->save();
        startAP();
    }
    
    // Switch to Client mode with new credentials
    void switchToClient(const char* ssid, const char* password) {
        WiFi.softAPdisconnect();
        delay(100);
        
        configMgr->setAPMode(false);
        configMgr->setSSID(ssid);
        configMgr->setPassword(password);
        configMgr->save();
        
        startClient();
    }
    
    // Getters
    WiFiStatus getStatus() const { return status; }
    bool isConnected() const { return status == WIFI_CONNECTED; }
    bool isAP() const { return status == WIFI_AP_MODE; }
    bool isConnecting() const { return status == WIFI_CONNECTING; }
    
    String getStatusString() const {
        switch (status) {
            case WIFI_AP_MODE: return "AP Mode";
            case WIFI_CONNECTING: return "Connecting...";
            case WIFI_CONNECTED: return "Connected";
            case WIFI_FAILED: return "Failed";
            default: return "Idle";
        }
    }
    
    String getIPAddress() const {
        if (status == WIFI_AP_MODE) {
            return WiFi.softAPIP().toString();
        } else if (status == WIFI_CONNECTED) {
            return WiFi.localIP().toString();
        }
        return "0.0.0.0";
    }
    
    int getRSSI() const {
        if (status == WIFI_CONNECTED) {
            return WiFi.RSSI();
        }
        return 0;
    }
};

#endif // WIFIMANAGER_H


#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// Configuration structure
struct SlapConfig {
    bool isAPMode;
    char ssid[32];
    char password[64];
    float threshold;
};

// Configuration manager class
class ConfigManager {
private:
    Preferences prefs;
    SlapConfig config;
    static constexpr const char* NAMESPACE = "slap-ai";
    
public:
    ConfigManager() {
        // Default values
        config.isAPMode = true;
        strcpy(config.ssid, "");
        strcpy(config.password, "");
        config.threshold = 1.0f;
    }
    
    // Load configuration from NVS
    bool load() {
        if (!prefs.begin(NAMESPACE, true)) { // readonly
            Serial.println("Failed to open preferences for reading");
            return false;
        }
        
        config.isAPMode = prefs.getBool("isAPMode", true);
        prefs.getString("ssid", config.ssid, sizeof(config.ssid));
        prefs.getString("password", config.password, sizeof(config.password));
        config.threshold = prefs.getFloat("threshold", 1.0f);
        
        prefs.end();
        
        Serial.println("Config loaded:");
        Serial.printf("  AP Mode: %s\n", config.isAPMode ? "YES" : "NO");
        Serial.printf("  SSID: %s\n", config.ssid);
        Serial.printf("  Threshold: %.2fg\n", config.threshold);
        
        return true;
    }
    
    // Save configuration to NVS
    bool save() {
        if (!prefs.begin(NAMESPACE, false)) { // read/write
            Serial.println("Failed to open preferences for writing");
            return false;
        }
        
        prefs.putBool("isAPMode", config.isAPMode);
        prefs.putString("ssid", config.ssid);
        prefs.putString("password", config.password);
        prefs.putFloat("threshold", config.threshold);
        
        prefs.end();
        
        Serial.println("Config saved successfully");
        return true;
    }
    
    // Reset to factory defaults
    void factoryReset() {
        if (!prefs.begin(NAMESPACE, false)) {
            Serial.println("Failed to open preferences for reset");
            return;
        }
        
        prefs.clear();
        prefs.end();
        
        // Reset to defaults
        config.isAPMode = true;
        strcpy(config.ssid, "");
        strcpy(config.password, "");
        config.threshold = 1.0f;
        
        Serial.println("Factory reset complete - settings cleared");
    }
    
    // Getters
    bool isAPMode() const { return config.isAPMode; }
    const char* getSSID() const { return config.ssid; }
    const char* getPassword() const { return config.password; }
    float getThreshold() const { return config.threshold; }
    
    // Setters
    void setAPMode(bool mode) { config.isAPMode = mode; }
    void setSSID(const char* s) { strncpy(config.ssid, s, sizeof(config.ssid) - 1); }
    void setPassword(const char* p) { strncpy(config.password, p, sizeof(config.password) - 1); }
    void setThreshold(float t) { config.threshold = t; }
    
    // Get full config for JSON responses
    const SlapConfig& getConfig() const { return config; }
};

#endif // CONFIG_H


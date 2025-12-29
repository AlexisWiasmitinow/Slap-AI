/*
 * WEMOS S3 MINI PRO - Slap Detector with WiFi Settings
 * 
 * Features:
 * - Motion detection (slap threshold configurable)
 * - WiFi AP mode (10.0.0.1) for configuration
 * - WiFi Client mode for home network
 * - Web interface for settings
 * - Factory reset via 5-second button press (IO47)
 */

#include <Arduino.h>
#include <Wire.h>
#include "QMI8658C.h"
#include "GC9A01A.h"
#include "Config.h"
#include "WiFiManager.h"
#include "WebServer.h"
#include "ButtonHandler.h"
#include "DisplayHelper.h"

// Display pins
#define TFT_CS   35
#define TFT_DC   36
#define TFT_RST  34
#define TFT_BL   33

// IMU pins
#define IMU_SDA 12
#define IMU_SCL 11

// Button pin
#define BUTTON_PIN 47

// LED
#define LED_PWR 7

// Objects
GC9A01A display(TFT_CS, TFT_DC, TFT_RST);
QMI8658C imu;
ConfigManager configMgr;
SlapWiFiManager wifiMgr(&configMgr);
SlapWebServer webServer(&configMgr, &wifiMgr);
ButtonHandler button(BUTTON_PIN, 5000);  // 5 second long press
DisplayHelper displayHelper(&display);

// Motion detection state
float peakMotion = 0.0;
unsigned long lastMotionTime = 0;
const unsigned long DISPLAY_TIMEOUT = 3000;  // 3 seconds

// Callback for factory reset
void onFactoryReset() {
    Serial.println("🔄 FACTORY RESET TRIGGERED");
    displayHelper.setState(DISPLAY_RESETTING);
    
    // Show reset animation
    for (int i = 0; i <= 10; i++) {
        displayHelper.showResetting(i / 10.0f);
        delay(100);
    }
    
    // Perform reset
    configMgr.factoryReset();
    delay(500);
    
    Serial.println("Restarting...");
    ESP.restart();
}

void setup() {
    Serial.begin(115200);
    
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(500);
    
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("========================================");
    Serial.println("===  SLAP-AI - WiFi Motion Detector  ===");
    Serial.println("========================================\n");
    
    // LED off
    pinMode(LED_PWR, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    
    // Initialize Display
    Serial.println("[1/5] Initializing Display...");
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Backlight ON
    
    display.begin();
    display.fillScreen(0x0000);  // Black
    Serial.println("      ✅ Display OK!");
    
    // Initialize IMU
    Serial.println("[2/5] Initializing IMU...");
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(400000);
    
    if (!imu.begin(Wire)) {
        Serial.println("      ❌ IMU FAILED!");
    } else {
        Serial.println("      ✅ IMU OK!");
    }
    
    // Load configuration
    Serial.println("[3/5] Loading Configuration...");
    configMgr.load();
    Serial.printf("      Threshold: %.2fg\n", configMgr.getThreshold());
    Serial.println("      ✅ Config OK!");
    
    // Initialize WiFi
    Serial.println("[4/5] Starting WiFi...");
    wifiMgr.begin();
    Serial.println("      ✅ WiFi started!");
    
    // Initialize Web Server
    Serial.println("[5/5] Starting Web Server...");
    webServer.begin();
    Serial.println("      ✅ Web server OK!");
    
    // Initialize Button Handler
    button.begin();
    button.onLongPress(onFactoryReset);
    
    // Show startup screen
    display.fillScreen(0xF800);  // Red flash
    delay(200);
    display.fillScreen(0x0000);  // Black
    
    Serial.println("\n========================================");
    Serial.println("  🚀 SYSTEM READY!");
    Serial.println("========================================\n");
    
    if (wifiMgr.isAP()) {
        Serial.println("📡 AP Mode Active:");
        Serial.println("   SSID: slap-ai (open)");
        Serial.printf("   IP: %s\n", wifiMgr.getIPAddress().c_str());
        Serial.println("   Connect and visit http://10.0.0.1\n");
    } else {
        Serial.println("📡 Connecting to WiFi...\n");
    }
}

void loop() {
    static unsigned long lastUpdate = 0;
    static float gravityX = 0, gravityY = 0, gravityZ = 1.0;
    
    // Update WiFi status
    wifiMgr.update();
    
    // Update button handler
    button.update();
    
    // Show button press progress on display
    if (button.isPressedNow()) {
        float progress = button.getLongPressProgress();
        if (progress > 0.01f) {  // Only show if actually being held
            displayHelper.showResetting(progress);
        }
    }
    
    // Display state management based on WiFi status
    static DisplayState lastWiFiState = DISPLAY_IDLE;
    DisplayState currentWiFiState = DISPLAY_IDLE;
    
    if (wifiMgr.isAP()) {
        currentWiFiState = DISPLAY_AP_MODE;
        if (lastWiFiState != DISPLAY_AP_MODE) {
            displayHelper.showAPMode(wifiMgr.getIPAddress().c_str());
        }
    } else if (wifiMgr.isConnecting()) {
        currentWiFiState = DISPLAY_CONNECTING;
        displayHelper.showConnecting(configMgr.getSSID());
    } else if (wifiMgr.isConnected()) {
        currentWiFiState = DISPLAY_CONNECTED;
        if (lastWiFiState != DISPLAY_CONNECTED) {
            displayHelper.showConnected();
            Serial.println("✅ WiFi connected - Slap detector active!");
        }
    }
    
    lastWiFiState = currentWiFiState;
    
    // Motion detection (only when connected or in AP mode, not while connecting)
    if (wifiMgr.isConnected() || wifiMgr.isAP()) {
        if (millis() - lastUpdate >= 100) {  // 10Hz update
            lastUpdate = millis();
            
            // Read IMU
            imu.update();
            IMUData data = imu.getData();
            
            // Low-pass filter for gravity estimation
            const float alpha = 0.8;
            gravityX = alpha * gravityX + (1 - alpha) * data.accelX;
            gravityY = alpha * gravityY + (1 - alpha) * data.accelY;
            gravityZ = alpha * gravityZ + (1 - alpha) * data.accelZ;
            
            // Linear acceleration (motion only, gravity removed)
            float linearX = data.accelX - gravityX;
            float linearY = data.accelY - gravityY;
            float linearZ = data.accelZ - gravityZ;
            
            // Motion magnitude
            float motionAccel = sqrt(linearX * linearX + 
                                    linearY * linearY + 
                                    linearZ * linearZ);
            
            // Get threshold from config
            float threshold = configMgr.getThreshold();
            
            // Motion detection
            bool motionDetected = (motionAccel > threshold);
            
            if (motionDetected) {
                if (motionAccel > peakMotion) {
                    peakMotion = motionAccel;
                    Serial.printf("💥 SLAP! Peak: %6.3fg (threshold: %.2fg)\n", 
                                 peakMotion, threshold);
                }
                lastMotionTime = millis();
            }
            
            // Check timeout
            unsigned long timeSinceMotion = millis() - lastMotionTime;
            bool displayActive = (timeSinceMotion < DISPLAY_TIMEOUT);
            
            if (!displayActive) {
                peakMotion = 0.0;
            }
            
            // Update display based on motion
            static bool wasMotionActive = false;
            
            if (displayActive && !wasMotionActive) {
                // Motion just detected - show SLAP
                displayHelper.showSlap();
            } else if (!displayActive && wasMotionActive) {
                // Motion timeout - return to appropriate state
                if (wifiMgr.isAP()) {
                    displayHelper.showAPMode(wifiMgr.getIPAddress().c_str());
                } else if (wifiMgr.isConnected()) {
                    displayHelper.showConnected();  // Black screen
                }
            }
            
            wasMotionActive = displayActive;
        }
    }
}

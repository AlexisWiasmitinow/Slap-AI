/*
 * Complete Hardware Diagnostic
 * Tests: I2C, GPIO, LED
 */

#include <Arduino.h>
#include <Wire.h>

#define LED_PWR 7
#define LED_DATA 8

void testLED() {
    Serial.println("\n[LED TEST]");
    Serial.println("Testing LED power pin (GPIO7)...");
    
    pinMode(LED_PWR, OUTPUT);
    
    Serial.println("  LED ON for 2 seconds...");
    digitalWrite(LED_PWR, HIGH);
    delay(2000);
    
    Serial.println("  LED OFF for 2 seconds...");
    digitalWrite(LED_PWR, LOW);
    delay(2000);
    
    Serial.println("  LED test complete\n");
}

void testI2CPins(int sda, int scl, const char* name) {
    Serial.printf("\n[%s] SDA=%d, SCL=%d\n", name, sda, scl);
    
    Wire.begin(sda, scl);
    Wire.setClock(100000);
    delay(100);
    
    Serial.println("  Scanning...");
    bool found = false;
    
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("  ✅ Device at 0x%02X\n", addr);
            found = true;
        }
    }
    
    if (!found) {
        Serial.println("  ❌ No devices");
    }
    
    Wire.end();
}

void setup() {
    Serial.begin(115200);
    
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(1000);
    
    for(int i=0; i<15; i++) Serial.println();
    
    Serial.println("==========================================");
    Serial.println("  COMPLETE HARDWARE DIAGNOSTIC");
    Serial.println("  WEMOS S3 MINI PRO");
    Serial.println("==========================================");
    
    // Test LED first
    testLED();
    
    // Test all reasonable I2C pin combinations for ESP32-S3
    Serial.println("\n[I2C SCAN - Testing all combinations]");
    
    testI2CPins(6, 5, "Config 1");
    testI2CPins(5, 6, "Config 2 (swapped)");
    testI2CPins(8, 9, "Config 3");
    testI2CPins(9, 8, "Config 4 (swapped)");
    testI2CPins(4, 5, "Config 5");
    testI2CPins(1, 2, "Config 6");
    testI2CPins(3, 4, "Config 7");
    testI2CPins(10, 11, "Config 8");
    testI2CPins(35, 36, "Config 9");
    testI2CPins(37, 38, "Config 10");
    testI2CPins(41, 42, "Config 11");
    
    Serial.println("\n==========================================");
    Serial.println("  DIAGNOSTIC COMPLETE");
    Serial.println("==========================================\n");
    
    Serial.println("📊 RESULTS SUMMARY:");
    Serial.println("  - LED test: Did you see the LED blink?");
    Serial.println("  - I2C scan: Were any devices found?");
    Serial.println("\n");
    
    Serial.println("💡 TROUBLESHOOTING:");
    Serial.println("  If NO I2C devices found:");
    Serial.println("    • IMU sensor may not be soldered");
    Serial.println("    • IMU power issue");
    Serial.println("    • Board variant without IMU");
    Serial.println("    • Check board documentation");
    Serial.println("\n");
    
    Serial.println("⏭️  NEXT STEPS:");
    Serial.println("  1. Verify your board has IMU installed");
    Serial.println("  2. Check WEMOS documentation for your exact model");
    Serial.println("  3. We can proceed with display-only version\n");
}

void loop() {
    delay(10000);
    Serial.println("Press RESET to run diagnostic again...");
}


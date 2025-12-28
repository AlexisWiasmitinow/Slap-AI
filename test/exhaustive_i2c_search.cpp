/*
 * Advanced I2C Test - Internal I2C and All GPIO Combinations
 * Tests EVERY possible GPIO pair as I2C
 */

#include <Arduino.h>
#include <Wire.h>

// List of all usable ESP32-S3 GPIO pins
int gpioPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};

void quickScan(int sda, int scl) {
    Wire.begin(sda, scl);
    Wire.setClock(100000);
    delay(50);
    
    // Quick scan for 0x6A and 0x6B only (QMI8658 addresses)
    for (byte addr = 0x6A; addr <= 0x6B; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("\n🎯 FOUND! SDA=%d, SCL=%d, Address=0x%02X\n", sda, scl, addr);
            
            // Read WHO_AM_I
            Wire.beginTransmission(addr);
            Wire.write(0x00);
            Wire.endTransmission(false);
            if (Wire.requestFrom(addr, (uint8_t)1) == 1) {
                byte id = Wire.read();
                Serial.printf("   WHO_AM_I: 0x%02X", id);
                if (id == 0x05) {
                    Serial.println(" ← QMI8658C CONFIRMED! ✅");
                } else {
                    Serial.println();
                }
            }
        }
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
    
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("========================================");
    Serial.println("  EXHAUSTIVE I2C PIN SEARCH");
    Serial.println("  WEMOS S3 MINI PRO");
    Serial.println("========================================\n");
    
    Serial.println("Testing ALL GPIO combinations...");
    Serial.println("This will take ~2 minutes.\n");
    Serial.println("Looking for QMI8658C at 0x6A or 0x6B...\n");
    
    int tested = 0;
    int pinCount = sizeof(gpioPins) / sizeof(gpioPins[0]);
    
    // Test all combinations
    for (int i = 0; i < pinCount; i++) {
        for (int j = 0; j < pinCount; j++) {
            if (i == j) continue;  // Skip same pin
            
            int sda = gpioPins[i];
            int scl = gpioPins[j];
            
            quickScan(sda, scl);
            tested++;
            
            // Progress indicator every 50 tests
            if (tested % 50 == 0) {
                Serial.printf("   ...tested %d combinations\n", tested);
            }
        }
    }
    
    Serial.println("\n========================================");
    Serial.println("  SEARCH COMPLETE");
    Serial.println("========================================\n");
    
    Serial.printf("Tested %d pin combinations\n\n", tested);
    
    Serial.println("📊 If you saw '🎯 FOUND!':");
    Serial.println("   → Note the SDA and SCL pin numbers!");
    Serial.println("   → We'll use those in the final code\n");
    
    Serial.println("❌ If NO devices found:");
    Serial.println("   → IMU hardware issue");
    Serial.println("   → Sensor not installed/working");
    Serial.println("   → We'll proceed with display-only version\n");
}

void loop() {
    delay(10000);
}


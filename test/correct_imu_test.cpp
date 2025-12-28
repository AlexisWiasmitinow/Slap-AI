/*
 * CORRECT IMU Test - Using GPIO 11/12 from schematic
 * NOTE: These pins are shared with TFT SPI!
 */

#include <Arduino.h>
#include <Wire.h>

#define IMU_SDA 12  // From schematic: IO12/SDA
#define IMU_SCL 11  // From schematic: IO11/SCL
#define IMU_INT1 18 // From schematic: IMU_INT1/IO18
#define IMU_INT2 21 // From schematic: IMU_INT2/IO21

void setup() {
    Serial.begin(115200);
    
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(1000);
    
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("========================================");
    Serial.println("  CORRECT I2C TEST - FROM SCHEMATIC");
    Serial.println("  WEMOS S3 MINI PRO");
    Serial.println("========================================\n");
    
    Serial.println("📋 Pin Configuration from Schematic:");
    Serial.println("   SDA = GPIO12");
    Serial.println("   SCL = GPIO11");
    Serial.println("   INT1 = GPIO18");
    Serial.println("   INT2 = GPIO21\n");
    
    Serial.println("⚠️  NOTE: GPIO11/12 are SHARED with TFT SPI!");
    Serial.println("   This requires careful initialization.\n");
    
    // Initialize I2C
    Serial.println("[1] Initializing I2C...");
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(400000);
    delay(100);
    Serial.println("    I2C initialized\n");
    
    // Scan for devices
    Serial.println("[2] Scanning I2C bus...");
    bool found = false;
    
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("    ✅ Device found at 0x%02X", addr);
            
            // Try to read WHO_AM_I register (0x00)
            Wire.beginTransmission(addr);
            Wire.write(0x00);
            error = Wire.endTransmission(false);
            
            if (error == 0 && Wire.requestFrom(addr, (uint8_t)1) == 1) {
                byte whoami = Wire.read();
                Serial.printf(" | WHO_AM_I: 0x%02X", whoami);
                if (whoami == 0x05) {
                    Serial.println(" ← QMI8658C! ✅✅✅");
                } else {
                    Serial.println();
                }
            } else {
                Serial.println();
            }
            found = true;
        }
    }
    
    if (!found) {
        Serial.println("    ❌ No I2C devices found\n");
        Serial.println("💡 Troubleshooting:");
        Serial.println("   • Pins might be held by SPI (TFT conflict)");
        Serial.println("   • Need to disable/release SPI first");
        Serial.println("   • IMU might be powered down");
    } else {
        Serial.println();
    }
    
    Serial.println("\n========================================");
    Serial.println("  TEST COMPLETE");
    Serial.println("========================================\n");
    
    Serial.println("🔍 Key Finding:");
    Serial.println("   GPIO 11 and 12 are SHARED between:");
    Serial.println("   • TFT SPI (MOSI=11, SCLK=12)");
    Serial.println("   • IMU I2C (SCL=11, SDA=12)");
    Serial.println("\n   This means we need to:");
    Serial.println("   1. Initialize one at a time");
    Serial.println("   2. Release pins when switching");
    Serial.println("   3. Or use multiplexing\n");
}

void loop() {
    delay(10000);
    Serial.println("Press RESET to run again...");
}


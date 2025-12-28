/*
 * WEMOS S3 MINI PRO - Enhanced I2C Diagnostic
 * Tests multiple I2C configurations to find the IMU
 */

#include <Arduino.h>
#include <Wire.h>

// Test multiple pin configurations
struct I2CConfig {
    int sda;
    int scl;
    const char* name;
};

I2CConfig configs[] = {
    {6, 5, "Default (SDA=6, SCL=5)"},
    {8, 9, "Alt 1 (SDA=8, SCL=9)"},
    {4, 5, "Alt 2 (SDA=4, SCL=5)"},
    {35, 36, "Alt 3 (SDA=35, SCL=36)"}
};

void scanI2C(TwoWire &wire) {
    Serial.println("   Scanning I2C bus...");
    int found = 0;
    
    for (byte addr = 1; addr < 127; addr++) {
        wire.beginTransmission(addr);
        byte error = wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("   ✅ FOUND device at 0x%02X\n", addr);
            found++;
            
            // Try to read WHO_AM_I register (0x00) for QMI8658
            wire.beginTransmission(addr);
            wire.write(0x00);
            error = wire.endTransmission(false);
            
            if (error == 0 && wire.requestFrom(addr, (uint8_t)1) == 1) {
                byte whoami = wire.read();
                Serial.printf("      WHO_AM_I register: 0x%02X", whoami);
                if (whoami == 0x05) {
                    Serial.println(" ← QMI8658C!");
                } else {
                    Serial.println();
                }
            }
        }
    }
    
    if (found == 0) {
        Serial.println("   ❌ No devices found");
    }
    Serial.println();
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
    Serial.println("  I2C DIAGNOSTIC TOOL");
    Serial.println("  WEMOS S3 MINI PRO");
    Serial.println("========================================\n");
    
    Serial.println("Testing multiple I2C pin configurations...\n");
    
    // Test each configuration
    for (int i = 0; i < sizeof(configs) / sizeof(configs[0]); i++) {
        Serial.printf("[%d] Testing: %s\n", i+1, configs[i].name);
        Serial.printf("    SDA=GPIO%d, SCL=GPIO%d\n", configs[i].sda, configs[i].scl);
        
        // Initialize I2C
        Wire.begin(configs[i].sda, configs[i].scl);
        Wire.setClock(100000); // Start with 100kHz (slow, more reliable)
        delay(100);
        
        // Scan
        scanI2C(Wire);
        
        // Test at 400kHz too
        Serial.println("   Trying 400kHz...");
        Wire.setClock(400000);
        delay(50);
        scanI2C(Wire);
        
        Wire.end();
        delay(200);
    }
    
    Serial.println("========================================");
    Serial.println("  DIAGNOSTIC COMPLETE");
    Serial.println("========================================\n");
    
    Serial.println("Next steps:");
    Serial.println("1. Check which configuration found devices");
    Serial.println("2. Look for address 0x6A or 0x6B (QMI8658)");
    Serial.println("3. If no devices found, check hardware\n");
}

void loop() {
    // Do nothing, just show results once
    delay(10000);
}


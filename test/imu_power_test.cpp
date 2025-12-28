/*
 * IMU Power and Enable Test
 * The IMU might need a power or enable pin activated first
 */

#include <Arduino.h>
#include <Wire.h>

// Potential power/enable pins to try
int powerPins[] = {7, 10, 21, 47, 48};

void scanI2C(int sda, int scl) {
    Wire.begin(sda, scl);
    Wire.setClock(100000);
    delay(100);
    
    bool found = false;
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("      ✅ FOUND device at 0x%02X", addr);
            
            // Try to read WHO_AM_I (register 0x00)
            Wire.beginTransmission(addr);
            Wire.write(0x00);
            error = Wire.endTransmission(false);
            
            if (error == 0 && Wire.requestFrom(addr, (uint8_t)1) == 1) {
                byte id = Wire.read();
                Serial.printf(" | WHO_AM_I: 0x%02X", id);
                if (id == 0x05) {
                    Serial.print(" ← QMI8658C!");
                }
            }
            Serial.println();
            found = true;
        }
    }
    
    if (!found) {
        Serial.println("      No devices found");
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
    
    Serial.println("========================================");
    Serial.println("  IMU POWER & ENABLE TEST");
    Serial.println("  WEMOS S3 MINI PRO");
    Serial.println("========================================\n");
    
    Serial.println("Strategy: Try enabling various power pins,");
    Serial.println("then scan I2C after each one.\n");
    
    // Test 1: No power pins enabled (baseline)
    Serial.println("[TEST 1] No power pins enabled");
    Serial.println("   I2C: SDA=GPIO6, SCL=GPIO5");
    scanI2C(6, 5);
    Serial.println();
    
    // Test 2-N: Try each potential power pin
    for (int i = 0; i < sizeof(powerPins)/sizeof(powerPins[0]); i++) {
        Serial.printf("[TEST %d] Enabling GPIO%d as power/enable\n", i+2, powerPins[i]);
        
        pinMode(powerPins[i], OUTPUT);
        
        // Try HIGH
        Serial.println("   Setting HIGH...");
        digitalWrite(powerPins[i], HIGH);
        delay(200);  // Give sensor time to power up
        
        Serial.println("   I2C: SDA=GPIO6, SCL=GPIO5");
        scanI2C(6, 5);
        
        // Try LOW (some pins might be active-low)
        Serial.println("   Setting LOW...");
        digitalWrite(powerPins[i], LOW);
        delay(200);
        
        Serial.println("   I2C: SDA=GPIO6, SCL=GPIO5");
        scanI2C(6, 5);
        
        Serial.println();
    }
    
    // Test with multiple pins
    Serial.println("[COMBO TEST] GPIO7=HIGH + GPIO10=HIGH");
    pinMode(7, OUTPUT);
    pinMode(10, OUTPUT);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    delay(200);
    
    Serial.println("   I2C: SDA=GPIO6, SCL=GPIO5");
    scanI2C(6, 5);
    Serial.println();
    
    // Final test: Maybe I2C pins are actually different
    Serial.println("[ALTERNATE PINS] Trying SDA=GPIO8, SCL=GPIO9");
    Serial.println("   With GPIO7=HIGH, GPIO10=HIGH");
    scanI2C(8, 9);
    Serial.println();
    
    Serial.println("[ALTERNATE PINS] Trying SDA=GPIO35, SCL=GPIO36");
    Serial.println("   With GPIO7=HIGH, GPIO10=HIGH");
    scanI2C(35, 36);
    Serial.println();
    
    Serial.println("========================================");
    Serial.println("  TEST COMPLETE");
    Serial.println("========================================\n");
    
    Serial.println("📊 If you saw '✅ FOUND device', note:");
    Serial.println("   • Which GPIO was set HIGH/LOW");
    Serial.println("   • Which I2C pins worked");
    Serial.println("   • The device address\n");
    
    Serial.println("❌ If NO devices found anywhere:");
    Serial.println("   • IMU might not be installed on your board");
    Serial.println("   • Check if there's a chip labeled QMI8658");
    Serial.println("   • Board might need different initialization\n");
}

void loop() {
    delay(10000);
    Serial.println("💡 Press RESET button to run test again...");
}


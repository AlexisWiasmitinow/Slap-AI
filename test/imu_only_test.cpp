/*
 * WEMOS S3 MINI PRO - IMU Only Test
 * Tests QMI8658C sensor without display
 */

#include <Arduino.h>
#include <Wire.h>
#include "QMI8658C.h"

// Pin definitions
#define IMU_SDA 6
#define IMU_SCL 5
#define LED_PWR 7

// Create IMU object
QMI8658C imu;

void setup() {
    // USB CDC Serial
    Serial.begin(115200);
    
    // Wait for Serial
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(500);
    
    // Clear screen
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("========================================");
    Serial.println("  WEMOS S3 - IMU ONLY TEST");
    Serial.println("========================================\n");
    
    // Turn off LED
    Serial.println("[1/3] Initializing LED...");
    pinMode(LED_PWR, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    Serial.println("      LED: OFF\n");
    
    // Initialize I2C
    Serial.println("[2/3] Initializing I2C...");
    Serial.printf("      SDA: GPIO%d\n", IMU_SDA);
    Serial.printf("      SCL: GPIO%d\n", IMU_SCL);
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(400000); // 400kHz
    Serial.println("      I2C: OK\n");
    
    // Scan I2C bus
    Serial.println("[3/3] Scanning I2C bus...");
    int devicesFound = 0;
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("      Found device at 0x%02X\n", address);
            devicesFound++;
        }
    }
    
    if (devicesFound == 0) {
        Serial.println("      No I2C devices found!");
        Serial.println("\n⚠️  CHECK: I2C wiring or sensor power");
    } else {
        Serial.printf("      Found %d device(s)\n", devicesFound);
    }
    Serial.println();
    
    // Initialize IMU
    Serial.println("[4/4] Initializing QMI8658C IMU...");
    if (!imu.begin(Wire)) {
        Serial.println("      ❌ IMU initialization FAILED!");
        Serial.println("\n⚠️  Possible issues:");
        Serial.println("      - Wrong I2C address (try 0x6A or 0x6B)");
        Serial.println("      - Sensor not responding");
        Serial.println("      - I2C wiring issue");
        Serial.println("\nContinuing anyway to show I2C scan results...\n");
    } else {
        Serial.println("      ✅ IMU initialization SUCCESS!");
    }
    
    Serial.println("\n========================================");
    Serial.println("  STARTING IMU DATA STREAM");
    Serial.println("========================================\n");
    delay(1000);
}

void loop() {
    static unsigned long lastUpdate = 0;
    static int counter = 0;
    
    if (millis() - lastUpdate >= 100) { // 10Hz update
        lastUpdate = millis();
        
        // Update IMU data
        imu.update();
        IMUData data = imu.getData();
        
        // Print counter and data
        Serial.printf("[%04d] ", counter++);
        Serial.printf("Accel: X=%6.3f Y=%6.3f Z=%6.3f g  |  ", 
                     data.accelX, data.accelY, data.accelZ);
        Serial.printf("Gyro: X=%7.2f Y=%7.2f Z=%7.2f °/s  |  ", 
                     data.gyroX, data.gyroY, data.gyroZ);
        Serial.printf("Temp: %5.1f°C\n", data.temperature);
        
        // Every 10 readings, show a separator
        if (counter % 10 == 0) {
            Serial.println("---");
        }
    }
}


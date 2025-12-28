/*
 * WEMOS S3 MINI PRO - IMU Only (Working Version)
 * Shows IMU data on serial monitor
 * 
 * Pins: SDA=GPIO12, SCL=GPIO11 (from schematic)
 */

#include <Arduino.h>
#include <Wire.h>
#include "QMI8658C.h"

// Correct pins from schematic
#define IMU_SDA 12
#define IMU_SCL 11
#define IMU_INT1 18
#define IMU_INT2 21

#define LED_PWR 7

// Create IMU object
QMI8658C imu;

void setup() {
    Serial.begin(115200);
    
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(500);
    
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("========================================");
    Serial.println("=== WEMOS S3 MINI PRO - IMU Test ===");
    Serial.println("========================================\n");
    
    // LED off
    pinMode(LED_PWR, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    Serial.println("LED: OFF\n");
    
    // Initialize I2C with correct pins
    Serial.println("Initializing I2C...");
    Serial.printf("  SDA = GPIO%d\n", IMU_SDA);
    Serial.printf("  SCL = GPIO%d\n", IMU_SCL);
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(400000);
    Serial.println("  I2C: OK\n");
    
    // Initialize IMU
    Serial.println("Initializing QMI8658C IMU...");
    if (!imu.begin(Wire)) {
        Serial.println("  ❌ IMU initialization FAILED!");
        Serial.println("\nSystem halted.");
        while(1) delay(1000);
    }
    
    Serial.println("  ✅ IMU initialized successfully!\n");
    
    Serial.println("========================================");
    Serial.println("  STREAMING IMU DATA (2Hz)");
    Serial.println("========================================\n");
    
    delay(1000);
}

void loop() {
    static unsigned long lastUpdate = 0;
    static int counter = 0;
    
    if (millis() - lastUpdate >= 500) {  // 2Hz (every 500ms)
        lastUpdate = millis();
        
        // Read IMU data
        imu.update();
        IMUData data = imu.getData();
        
        // Print nicely formatted data
        Serial.printf("\n[Reading #%d]\n", counter++);
        Serial.println("┌─────────────────────────────────────┐");
        
        Serial.println("│ ACCELEROMETER (g-force)             │");
        Serial.printf("│   X: %7.3f g                    │\n", data.accelX);
        Serial.printf("│   Y: %7.3f g                    │\n", data.accelY);
        Serial.printf("│   Z: %7.3f g                    │\n", data.accelZ);
        Serial.println("│                                     │");
        
        Serial.println("│ GYROSCOPE (degrees/second)          │");
        Serial.printf("│   X: %8.2f °/s                │\n", data.gyroX);
        Serial.printf("│   Y: %8.2f °/s                │\n", data.gyroY);
        Serial.printf("│   Z: %8.2f °/s                │\n", data.gyroZ);
        Serial.println("│                                     │");
        
        Serial.printf("│ TEMPERATURE: %5.1f °C               │\n", data.temperature);
        Serial.println("└─────────────────────────────────────┘");
    }
}

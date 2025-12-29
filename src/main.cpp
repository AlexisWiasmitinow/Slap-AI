/*
 * WEMOS S3 MINI PRO - With Custom GC9A01A Driver
 * Uses Adafruit GFX instead of TFT_eSPI
 */

#include <Arduino.h>
#include <Wire.h>
#include "QMI8658C.h"
#include "GC9A01A.h"

// Display pins from schematic
#define TFT_CS   35
#define TFT_DC   36
#define TFT_RST  34
#define TFT_BL   33

// IMU pins
#define IMU_SDA 12
#define IMU_SCL 11

// LED
#define LED_PWR 7

// Color definitions (RGB565)
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Objects
GC9A01A display(TFT_CS, TFT_DC, TFT_RST);
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
    Serial.println("=== WEMOS S3 MINI PRO - GFX Test ===");
    Serial.println("========================================\n");
    
    // LED off
    pinMode(LED_PWR, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    
    // Initialize IMU
    Serial.println("[1/2] Initializing IMU...");
    Wire.begin(IMU_SDA, IMU_SCL);
    Wire.setClock(400000);
    
    if (!imu.begin(Wire)) {
        Serial.println("      ❌ IMU FAILED!");
    } else {
        Serial.println("      ✅ IMU OK!");
    }
    
    // Initialize Display
    Serial.println("[2/2] Initializing Display...");
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Backlight ON
    
    display.begin();
    Serial.println("      Display initialized");
    
    display.fillScreen(RED);
    delay(500);
    display.fillScreen(BLACK);
    
    Serial.println("      ✅ Display OK!\n");
    
    // Show startup text
    display.setTextColor(CYAN);
    display.setTextSize(2);
    display.setCursor(20, 50);
    display.print("WEMOS");
    display.setCursor(15, 70);
    display.print("S3 PRO");
    
    delay(2000);
    display.fillScreen(BLACK);
    
    Serial.println("========================================");
    Serial.println("  READY! Showing IMU data...");
    Serial.println("========================================\n");
}

void loop() {
    static unsigned long lastUpdate = 0;
    
    if (millis() - lastUpdate >= 500) {  // 2Hz
        lastUpdate = millis();
        
        // Read IMU
        imu.update();
        IMUData data = imu.getData();
        
        // Serial output
        Serial.printf("Accel: X=%6.3f Y=%6.3f Z=%6.3f | Gyro: X=%7.2f Y=%7.2f Z=%7.2f\n",
                     data.accelX, data.accelY, data.accelZ,
                     data.gyroX, data.gyroY, data.gyroZ);
        
        // Display output - clear old values first
        display.setTextSize(1);
        
        // Title (only draw once)
        static bool firstDraw = true;
        if (firstDraw) {
            display.setTextColor(CYAN);
            display.setCursor(5, 5);
            display.print("=== IMU DATA ===");
            
            display.setTextColor(YELLOW);
            display.setCursor(5, 25);
            display.print("Accel (g):");
            
            display.setCursor(5, 75);
            display.print("Gyro (d/s):");
            
            firstDraw = false;
        }
        
        // Clear and draw accelerometer values
        display.fillRect(0, 38, 128, 30, BLACK);  // Clear accel area
        display.setTextColor(WHITE);
        display.setCursor(5, 38);
        display.print("X:");
        display.print(data.accelX, 3);
        
        display.setCursor(5, 48);
        display.print("Y:");
        display.print(data.accelY, 3);
        
        display.setCursor(5, 58);
        display.print("Z:");
        display.print(data.accelZ, 3);
        
        // Clear and draw gyroscope values
        display.fillRect(0, 88, 128, 30, BLACK);  // Clear gyro area
        display.setTextColor(WHITE);
        display.setCursor(5, 88);
        display.print("X:");
        display.print(data.gyroX, 2);
        
        display.setCursor(5, 98);
        display.print("Y:");
        display.print(data.gyroY, 2);
        
        display.setCursor(5, 108);
        display.print("Z:");
        display.print(data.gyroZ, 2);
        
        // Clear and draw temperature
        display.fillRect(0, 120, 128, 8, BLACK);  // Clear temp area
        display.setTextColor(GREEN);
        display.setCursor(5, 120);
        display.print("T:");
        display.print(data.temperature, 1);
        display.print("C");
    }
}

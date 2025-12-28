/*
 * Simple Display Test - for WEMOS S3 MINI PRO
 * Tests TFT display with minimal code
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

#define TFT_BL  3

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n=== Display Test ===");
    
    // Backlight ON
    Serial.println("1. Setting backlight HIGH");
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    delay(500);
    
    // Initialize display
    Serial.println("2. Initializing TFT...");
    tft.init();
    Serial.println("   TFT init done");
    
    // Test 1: RED screen
    Serial.println("3. RED screen test");
    tft.fillScreen(TFT_RED);
    delay(2000);
    
    // Test 2: GREEN screen
    Serial.println("4. GREEN screen test");
    tft.fillScreen(TFT_GREEN);
    delay(2000);
    
    // Test 3: BLUE screen
    Serial.println("5. BLUE screen test");
    tft.fillScreen(TFT_BLUE);
    delay(2000);
    
    // Test 4: WHITE screen
    Serial.println("6. WHITE screen test");
    tft.fillScreen(TFT_WHITE);
    delay(2000);
    
    // Test 5: BLACK screen
    Serial.println("7. BLACK screen test");
    tft.fillScreen(TFT_BLACK);
    delay(1000);
    
    // Test 6: Text
    Serial.println("8. Text test");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.println("HELLO!");
    
    Serial.println("9. Test complete!");
    Serial.println("\nDid you see colors on the display?");
    Serial.println("If YES: Display works!");
    Serial.println("If NO: Try backlight LOW or check pins");
}

void loop() {
    // Blink colors slowly
    static unsigned long lastChange = 0;
    static int colorIndex = 0;
    
    if (millis() - lastChange > 3000) {
        lastChange = millis();
        
        switch(colorIndex) {
            case 0: 
                tft.fillScreen(TFT_RED); 
                Serial.println("RED");
                break;
            case 1: 
                tft.fillScreen(TFT_GREEN); 
                Serial.println("GREEN");
                break;
            case 2: 
                tft.fillScreen(TFT_BLUE); 
                Serial.println("BLUE");
                break;
        }
        
        colorIndex = (colorIndex + 1) % 3;
    }
}


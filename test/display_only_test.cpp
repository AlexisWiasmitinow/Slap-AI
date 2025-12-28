/*
 * WEMOS S3 MINI PRO - Display Only Test
 * Tests just the display without IMU
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

#define TFT_BL  3
#define LED_PWR 7

TFT_eSPI tft = TFT_eSPI();

void setup() {
    // USB CDC Serial
    Serial.begin(115200);
    
    // Wait for Serial with timeout
    unsigned long start = millis();
    while (!Serial && (millis() - start < 3000)) {
        delay(100);
    }
    delay(500);
    
    // Lots of newlines to ensure visibility
    for(int i=0; i<10; i++) Serial.println();
    
    Serial.println("====================================");
    Serial.println("  WEMOS S3 - DISPLAY TEST");
    Serial.println("====================================");
    
    // Turn off LED
    Serial.println("[1/6] Initializing LED...");
    pinMode(LED_PWR, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    Serial.println("      LED: OK");
    
    // Try backlight HIGH first
    Serial.println("[2/6] Setting backlight HIGH...");
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    delay(200);
    Serial.println("      Backlight: HIGH");
    
    // Initialize display
    Serial.println("[3/6] Calling tft.init()...");
    Serial.flush(); // Make sure this prints before potential crash
    delay(100);
    
    tft.init();
    
    Serial.println("      TFT init: OK");
    
    // Set rotation
    Serial.println("[4/6] Setting rotation...");
    tft.setRotation(0);
    Serial.println("      Rotation: 0");
    
    // Fill screen RED (test)
    Serial.println("[5/6] Filling screen RED...");
    Serial.flush();
    tft.fillScreen(TFT_RED);
    delay(1000);
    Serial.println("      RED screen: OK");
    
    // Fill screen BLACK
    Serial.println("[6/6] Filling screen BLACK...");
    tft.fillScreen(TFT_BLACK);
    Serial.println("      BLACK screen: OK");
    
    // Draw text
    Serial.println("[7/6] Drawing text...");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.println("HELLO!");
    Serial.println("      Text: OK");
    
    Serial.println("\n====================================");
    Serial.println("  DISPLAY TEST COMPLETE!");
    Serial.println("====================================");
    Serial.println("\nDid you see:");
    Serial.println("  1. RED flash?");
    Serial.println("  2. BLACK screen?");
    Serial.println("  3. White 'HELLO!' text?");
    Serial.println("\nIf NO, trying backlight LOW...\n");
    
    // Try backlight LOW too
    delay(2000);
    Serial.println("Trying backlight LOW...");
    digitalWrite(TFT_BL, LOW);
    delay(2000);
    Serial.println("Backlight back to HIGH...");
    digitalWrite(TFT_BL, HIGH);
}

void loop() {
    // Cycle colors
    static unsigned long lastChange = 0;
    static int colorIndex = 0;
    static const char* colorNames[] = {"RED", "GREEN", "BLUE", "WHITE", "BLACK"};
    static const uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_WHITE, TFT_BLACK};
    
    if (millis() - lastChange > 2000) {
        lastChange = millis();
        
        Serial.print("Showing: ");
        Serial.println(colorNames[colorIndex]);
        
        tft.fillScreen(colors[colorIndex]);
        
        if (colorIndex == 4) { // BLACK - show text
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(20, 50);
            tft.println("WORKING!");
        }
        
        colorIndex = (colorIndex + 1) % 5;
    }
}


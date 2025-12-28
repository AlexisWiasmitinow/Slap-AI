/*
 * Minimal Serial Test - ESP32-S3 USB CDC
 * This tests if the board can communicate via USB
 */

#include <Arduino.h>

void setup() {
    // ESP32-S3 USB CDC needs time to initialize
    Serial.begin(115200);
    
    // Critical: Wait for USB CDC to be ready
    delay(3000);
    
    // Send multiple test messages
    Serial.println("\n\n\n");
    Serial.println("===================");
    Serial.println("SERIAL TEST - START");
    Serial.println("===================");
    Serial.println("If you see this, USB CDC works!");
    Serial.println("");
    
    // Test repeated output
    for (int i = 0; i < 5; i++) {
        Serial.print("Test message ");
        Serial.println(i);
        delay(200);
    }
    
    Serial.println("");
    Serial.println("Setup complete!");
}

void loop() {
    static int counter = 0;
    Serial.print("Loop: ");
    Serial.println(counter++);
    delay(1000);
}


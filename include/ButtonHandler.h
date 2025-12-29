#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
private:
    uint8_t pin;
    bool lastState;
    unsigned long pressStartTime;
    unsigned long longPressDuration;
    bool longPressTriggered;
    bool isPressed;
    
    // Callback function for long press
    void (*longPressCallback)();
    
public:
    ButtonHandler(uint8_t buttonPin, unsigned long longPressMs = 5000) 
        : pin(buttonPin), 
          longPressDuration(longPressMs),
          lastState(HIGH),
          pressStartTime(0),
          longPressTriggered(false),
          isPressed(false),
          longPressCallback(nullptr) {
    }
    
    void begin() {
        pinMode(pin, INPUT_PULLUP); // Button to GND, internal pullup
        lastState = digitalRead(pin);
        Serial.printf("Button handler initialized on pin %d\n", pin);
    }
    
    // Set callback for long press event
    void onLongPress(void (*callback)()) {
        longPressCallback = callback;
    }
    
    // Call this in loop() to check button state
    void update() {
        bool currentState = digitalRead(pin);
        
        // Button pressed (LOW because of pullup)
        if (currentState == LOW && lastState == HIGH) {
            // Button just pressed
            pressStartTime = millis();
            longPressTriggered = false;
            isPressed = true;
            Serial.println("Button pressed");
        }
        
        // Button held down
        if (currentState == LOW && isPressed) {
            unsigned long pressDuration = millis() - pressStartTime;
            
            // Check for long press
            if (pressDuration >= longPressDuration && !longPressTriggered) {
                longPressTriggered = true;
                Serial.println("Long press detected!");
                
                if (longPressCallback != nullptr) {
                    longPressCallback();
                }
            }
        }
        
        // Button released
        if (currentState == HIGH && lastState == LOW) {
            isPressed = false;
            Serial.println("Button released");
        }
        
        lastState = currentState;
    }
    
    // Get current press duration in ms (returns 0 if not pressed)
    unsigned long getPressDuration() const {
        if (isPressed) {
            return millis() - pressStartTime;
        }
        return 0;
    }
    
    // Check if button is currently being pressed
    bool isPressedNow() const {
        return isPressed;
    }
    
    // Get progress of long press (0.0 to 1.0)
    float getLongPressProgress() const {
        if (isPressed && !longPressTriggered) {
            unsigned long duration = millis() - pressStartTime;
            return min(1.0f, (float)duration / (float)longPressDuration);
        }
        return 0.0f;
    }
};

#endif // BUTTONHANDLER_H


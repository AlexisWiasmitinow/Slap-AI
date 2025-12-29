#ifndef DISPLAYHELPER_H
#define DISPLAYHELPER_H

#include "GC9A01A.h"
#include <Arduino.h>

// Color definitions (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_CYAN    0x07FF
#define COLOR_YELLOW  0xFFE0
#define COLOR_WHITE   0xFFFF
#define COLOR_ORANGE  0xFD20

enum DisplayState {
    DISPLAY_IDLE,
    DISPLAY_AP_MODE,
    DISPLAY_CONNECTING,
    DISPLAY_CONNECTED,
    DISPLAY_SLAP,
    DISPLAY_RESETTING
};

class DisplayHelper {
private:
    GC9A01A* display;
    DisplayState currentState;
    DisplayState lastState;
    
    void centerText(const char* text, int y, int textSize) {
        display->setTextSize(textSize);
        int16_t x1, y1;
        uint16_t w, h;
        display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
        int x = (128 - w) / 2;  // Center on 128px display
        display->setCursor(x, y);
        display->print(text);
    }
    
public:
    DisplayHelper(GC9A01A* disp) : display(disp), currentState(DISPLAY_IDLE), lastState(DISPLAY_IDLE) {}
    
    void showAPMode(const char* ip) {
        if (currentState == DISPLAY_AP_MODE && lastState == DISPLAY_AP_MODE) {
            return; // Already showing, don't redraw
        }
        
        display->fillScreen(COLOR_BLACK);
        display->setTextColor(COLOR_CYAN);
        
        // Title
        display->setTextSize(2);
        centerText("AP MODE", 20, 2);
        
        // SSID
        display->setTextColor(COLOR_WHITE);
        display->setTextSize(1);
        centerText("slap-ai", 50, 1);
        centerText("(open)", 65, 1);
        
        // IP
        display->setTextColor(COLOR_GREEN);
        display->setTextSize(1);
        centerText("Connect to:", 85, 1);
        display->setTextSize(2);
        centerText(ip, 100, 2);
        
        currentState = DISPLAY_AP_MODE;
        lastState = DISPLAY_AP_MODE;
    }
    
    void showConnecting(const char* ssid) {
        if (currentState == DISPLAY_CONNECTING && lastState == DISPLAY_CONNECTING) {
            // Show animation dots
            static int dotCount = 0;
            static unsigned long lastDot = 0;
            
            if (millis() - lastDot > 500) {
                lastDot = millis();
                dotCount = (dotCount + 1) % 4;
                
                // Clear dot area
                display->fillRect(0, 95, 128, 20, COLOR_BLACK);
                
                display->setTextColor(COLOR_YELLOW);
                display->setTextSize(2);
                String dots = "";
                for (int i = 0; i < dotCount; i++) {
                    dots += ".";
                }
                centerText(dots.c_str(), 95, 2);
            }
            return;
        }
        
        display->fillScreen(COLOR_BLACK);
        display->setTextColor(COLOR_YELLOW);
        
        display->setTextSize(1);
        centerText("Connecting to:", 30, 1);
        
        display->setTextColor(COLOR_WHITE);
        display->setTextSize(2);
        centerText(ssid, 50, 2);
        
        display->setTextColor(COLOR_YELLOW);
        display->setTextSize(2);
        centerText(".", 95, 2);
        
        currentState = DISPLAY_CONNECTING;
        lastState = DISPLAY_CONNECTING;
    }
    
    void showConnected() {
        display->fillScreen(COLOR_BLACK);
        currentState = DISPLAY_CONNECTED;
        lastState = DISPLAY_CONNECTED;
    }
    
    void showSlap() {
        if (currentState == DISPLAY_SLAP && lastState == DISPLAY_SLAP) {
            return; // Already showing
        }
        
        display->fillScreen(COLOR_RED);
        display->setTextColor(COLOR_BLACK);
        display->setTextSize(3);
        centerText("SLAP!", 55, 3);
        
        currentState = DISPLAY_SLAP;
        lastState = DISPLAY_SLAP;
    }
    
    void showResetting(float progress) {
        if (currentState != DISPLAY_RESETTING) {
            display->fillScreen(COLOR_BLACK);
            display->setTextColor(COLOR_ORANGE);
            display->setTextSize(2);
            centerText("RESET", 40, 2);
            currentState = DISPLAY_RESETTING;
        }
        
        // Progress bar
        int barWidth = 100;
        int barHeight = 10;
        int barX = (128 - barWidth) / 2;
        int barY = 70;
        
        // Border
        display->drawRect(barX - 2, barY - 2, barWidth + 4, barHeight + 4, COLOR_WHITE);
        
        // Fill
        int fillWidth = (int)(barWidth * progress);
        display->fillRect(barX, barY, fillWidth, barHeight, COLOR_ORANGE);
        
        lastState = DISPLAY_RESETTING;
    }
    
    void showIdle() {
        if (currentState == DISPLAY_IDLE && lastState == DISPLAY_IDLE) {
            return;
        }
        
        display->fillScreen(COLOR_BLACK);
        currentState = DISPLAY_IDLE;
        lastState = DISPLAY_IDLE;
    }
    
    DisplayState getState() const {
        return currentState;
    }
    
    void setState(DisplayState state) {
        currentState = state;
    }
};

#endif // DISPLAYHELPER_H


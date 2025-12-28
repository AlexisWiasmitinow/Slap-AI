# Display Troubleshooting Guide

## Problem: Display is Off/Black

If your code uploads successfully but the display stays black, try these solutions:

---

## 🔧 **Solution 1: Check Serial Output First**

1. **Open Serial Monitor** (115200 baud):
   ```powershell
   pio device monitor
   ```

2. **Look for output**:
   ```
   === WEMOS S3 MINI PRO - IMU Test ===
   Starting initialization...
   LED power initialized
   Initializing TFT display...
   ```

3. **If you see output**: Code is running, display issue only
   **If no output**: Code might be crashing - see below

---

## 🔧 **Solution 2: Try Inverted Backlight**

The backlight might be active-LOW instead of active-HIGH:

Edit `src/main.cpp`, change line:
```cpp
digitalWrite(TFT_BL, HIGH); // Turn on backlight
```
To:
```cpp
digitalWrite(TFT_BL, LOW); // Turn on backlight (inverted)
```

Then rebuild and upload.

---

## 🔧 **Solution 3: Run Simple Display Test**

Let's test the display with minimal code:

1. **Temporarily rename main.cpp**:
   ```powershell
   Rename-Item src/main.cpp src/main.cpp.backup
   ```

2. **Copy the test file**:
   ```powershell
   Copy-Item test/display_test.cpp src/main.cpp
   ```

3. **Build and upload**:
   ```powershell
   pio run --target upload
   ```

4. **Watch serial output** - it will tell you what to expect on screen

5. **After testing, restore original**:
   ```powershell
   Remove-Item src/main.cpp
   Rename-Item src/main.cpp.backup src/main.cpp
   ```

---

## 🔧 **Solution 4: Try Different Display Driver**

The board might use GC9107 instead of GC9A01. Edit `include/User_Setup.h`:

**Current setting:**
```cpp
#define GC9A01_DRIVER
```

**Try this instead:**
```cpp
// #define GC9A01_DRIVER  // Comment out

// Try ST7735 driver
#define ST7735_DRIVER
#define CGRAM_OFFSET
#define TFT_RGB_ORDER TFT_BGR
```

---

## 🔧 **Solution 5: Verify Pin Connections**

According to the schematic, pins should be:

| Function | GPIO | Check |
|----------|------|-------|
| MOSI     | 11   | ✓     |
| SCLK     | 12   | ✓     |
| CS       | 4    | ✓     |
| DC       | 2    | ✓     |
| RST      | 1    | ✓     |
| BL       | 3    | ✓     |

If your board variant has different pins, update `include/User_Setup.h`.

---

## 🔧 **Solution 6: Check for ESP32-S3 USB/Serial Issues**

The ESP32-S3 USB CDC can sometimes interfere. Try adding a longer delay:

In `src/main.cpp`, change:
```cpp
Serial.begin(115200);
delay(2000);  // Increase from 1000 to 2000
```

---

## 🔧 **Solution 7: Reduce SPI Frequency**

Sometimes displays are sensitive to high frequencies. Edit `include/User_Setup.h`:

Change:
```cpp
#define SPI_FREQUENCY  27000000  // 27 MHz
```
To:
```cpp
#define SPI_FREQUENCY  20000000  // 20 MHz (slower, more stable)
```

---

## 🔧 **Solution 8: Try Different Rotation**

The display might be initialized in a weird orientation. In `src/main.cpp`:

Try different rotation values (0, 1, 2, or 3):
```cpp
tft.setRotation(0);  // Try 0, 1, 2, or 3
```

---

## 📊 **What You Should See**

### On Serial Monitor:
```
=== WEMOS S3 MINI PRO - IMU Test ===
Starting initialization...
LED power initialized
Initializing TFT display...
Setting backlight pin HIGH...
Calling tft.init()...
TFT init complete
Setting rotation...
Filling screen with red (test)...
Filling screen with black...
Display initialized!
```

### On TFT Display:
1. **Red flash** (1 second) - confirms display is working
2. **Black screen** with text appearing
3. **"WEMOS S3 IMU Test"** startup text

---

## 🔍 **Diagnostic Checklist**

Run through these:

- [ ] Serial output appears (code is running)
- [ ] Backlight tried both HIGH and LOW
- [ ] Display test program tried
- [ ] Different drivers tried (GC9A01 vs ST7735)
- [ ] SPI frequency reduced
- [ ] USB cable is data-capable (not power-only)
- [ ] Board is the correct variant (LOLIN S3 MINI PRO)

---

## 📝 **Report Back**

If still not working, tell me:
1. What do you see on serial monitor?
2. Does the display glow/backlight turn on?
3. Is there any flicker when it starts?
4. Which solutions did you try?

---

## 🎯 **Most Common Fixes**

**90% of display issues are:**
1. **Backlight polarity** (HIGH vs LOW)
2. **Wrong driver** (GC9A01 vs GC9107)
3. **Pin mismatch** (board variant difference)

Try solutions 2, 3, and 4 first!


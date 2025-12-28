# Troubleshooting: MicroPython Pre-installed

## Problem

The WEMOS S3 MINI PRO comes with **MicroPython pre-installed** by default. When you try to upload Arduino firmware, you might see:

```
MicroPython v1.24.1-1.g50c8864e7f on 2024-12-18; LOLIN S3 MINI PRO with ESP32S3
Type "help()" for more information.
```

## Solution Options

### ✅ **Option 1: BOOT Button Method** (Recommended)

This is the **easiest and safest** method:

1. **Start the upload**:
   ```powershell
   pio run --target upload
   ```

2. **Watch for "Connecting..."** in the output

3. **Immediately HOLD the BOOT button** (the button labeled IO0 or BOOT on the board)

4. **Keep holding** for 2-3 seconds until you see:
   ```
   Chip is ESP32-S3
   ```

5. **Release** the button - upload will continue automatically

### ✅ **Option 2: Erase Flash First**

If the BOOT button method doesn't work, erase the entire flash:

1. **Using the build script**:
   ```powershell
   .\build.ps1
   # Choose option 6 (Erase flash)
   # HOLD BOOT button when prompted
   ```

2. **Or manually**:
   ```powershell
   # HOLD BOOT button, then run:
   pio run --target erase
   
   # Wait for completion, then upload:
   pio run --target upload
   ```

3. **Or with esptool**:
   ```powershell
   # Install esptool
   pip install esptool
   
   # HOLD BOOT button, then:
   esptool.py --chip esp32s3 --port COM16 erase_flash
   
   # Then upload normally
   pio run --target upload
   ```

### ✅ **Option 3: BOOT + RESET Combo**

If your board has a RESET button:

1. **Hold BOOT button** (IO0) - keep holding
2. **Press and release RESET button** while still holding BOOT
3. **Release BOOT button** after 1 second
4. Now the board is in bootloader mode
5. **Run upload**: `pio run --target upload`

## Understanding the Boot Process

### Normal Boot (MicroPython runs):
- GPIO0 (BOOT) is HIGH during startup
- ESP32-S3 boots from flash
- MicroPython starts automatically

### Download/Bootloader Mode (for uploading):
- GPIO0 (BOOT) is LOW during startup
- ESP32-S3 enters bootloader mode
- Ready to receive new firmware via USB

## Visual Guide

```
┌─────────────────────────────────────┐
│  WEMOS S3 MINI PRO (top view)       │
│                                     │
│  [USB-C Port]                       │
│                                     │
│  [BOOT]  ← This is GPIO0            │
│  [IO47]                             │
│  [IO48]                             │
│                                     │
│  [TFT Screen]                       │
│                                     │
│  [Antenna area]                     │
└─────────────────────────────────────┘
```

## Step-by-Step Upload Process

### **Complete Process:**

1. **Build first** (optional but recommended):
   ```powershell
   pio run
   ```
   Wait for build to complete successfully.

2. **Prepare to upload**:
   ```powershell
   pio run --target upload
   ```

3. **Watch the output** - you'll see:
   ```
   Configuring upload protocol...
   AVAILABLE: cmsis-dap, esp-bridge, esp-builtin, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
   CURRENT: upload_protocol = esptool
   Looking for upload port...
   Auto-detected: COM16
   Uploading .pio\build\lolin_s3_mini\firmware.bin
   esptool.py v4.7.0
   Serial port COM16
   Connecting...
   ```

4. **RIGHT NOW** - when you see "Connecting..." with dots appearing:
   ```
   Connecting........___
   ```
   **HOLD the BOOT button!**

5. **You'll see**:
   ```
   Chip is ESP32-S3 (revision v0.2)
   Features: WiFi, BLE
   Crystal is 40MHz
   MAC: xx:xx:xx:xx:xx:xx
   Uploading stub...
   Running stub...
   Stub running...
   Changing baud rate to 460800
   Changed.
   ```
   Now you can **release the BOOT button**.

6. **Upload continues**:
   ```
   Writing at 0x00010000... (10 %)
   Writing at 0x00020000... (20 %)
   ...
   Writing at 0x00070000... (100 %)
   Wrote 123456 bytes at 0x00010000 in 2.5 seconds
   ```

7. **Done!** The board will automatically reset and run your Arduino code.

## Common Issues

### Issue: "Timed out waiting for packet header"
```
A serial exception error occurred: Timed out waiting for packet header
```
**Solution**: You didn't hold BOOT button in time. Try again and hold it earlier.

### Issue: "Permission denied" or "Port is busy"
```
PermissionError(13, 'Access is denied')
```
**Solution**: 
1. Close any serial monitor windows
2. Close Device Manager if open
3. Unplug and replug the USB cable
4. Try again

### Issue: Upload succeeds but MicroPython still shows
**Solution**: The upload didn't actually complete. Try erasing flash first (Option 2).

### Issue: COM port not detected
**Solution**:
1. Try a different USB cable (some are power-only)
2. Check Device Manager for "USB JTAG/serial debug unit"
3. Install ESP32-S3 USB drivers if needed

## After Successful Upload

Once uploaded successfully, you should see on the TFT screen:
```
WEMOS S3
IMU Test
Init IMU...
IMU OK!
```

And then the real-time IMU data display.

## Reverting to MicroPython (if needed)

If you want to go back to MicroPython:
1. Download MicroPython firmware for LOLIN S3 MINI PRO
2. Use esptool to flash it:
   ```powershell
   esptool.py --chip esp32s3 --port COM16 write_flash -z 0x0 firmware.bin
   ```

## Quick Reference Commands

```powershell
# Build only
pio run

# Upload (remember BOOT button!)
pio run --target upload

# Erase flash (removes MicroPython)
pio run --target erase

# Monitor serial
pio device monitor

# All in one (build + upload + monitor)
pio run --target upload && pio device monitor

# Use the helper script
.\build.ps1
```

## Still Having Issues?

1. **Check USB cable** - try a different one
2. **Check COM port** - verify in Device Manager
3. **Try different USB port** on your computer
4. **Check board** - LED should light up when connected
5. **Reinstall USB drivers** - search for "ESP32-S3 USB driver"

---

**Key Takeaway**: Always **HOLD the BOOT button** when you see "Connecting..." during upload!


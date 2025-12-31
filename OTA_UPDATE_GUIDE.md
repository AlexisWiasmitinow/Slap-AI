# 🔄 OTA Firmware Update Guide

## Overview

Your Slap-AI device now supports **Over-The-Air (OTA)** firmware updates! Update your device wirelessly with a simple drag-and-drop interface - no USB cable required! ✨

## ✨ Features

- **Drag & Drop Interface** - Just drop your firmware file onto the web page
- **Visual Feedback** - Glowing drop zone with animations
- **Real-time Progress** - See upload progress and status
- **Automatic Reboot** - Device restarts automatically after successful update
- **Error Handling** - Clear error messages if something goes wrong

## 📋 Quick Start

### 1. Build Your Firmware

```powershell
# Build the firmware
pio run

# The firmware is automatically copied to:
# firmware_with_ota.bin (in project root)
```

### 2. Access the Web Interface

**In AP Mode:**
- Connect to `slap-ai` WiFi network
- Open browser to: http://10.0.0.1

**In Client Mode (Connected to WiFi):**
- Open browser to: **http://slap-ai.local** ✨
- Or use the device's IP address from your router

### 3. Upload Firmware

1. Scroll down to the **"🔄 Firmware Update"** section
2. **Drag and drop** your `.bin` file onto the drop zone
   - OR click the drop zone to browse and select the file
3. Confirm the upload
4. Watch the progress bar - it shows:
   - Upload percentage
   - Bytes uploaded / total bytes
5. Wait for "✓ Upload complete! Device restarting..."
6. Device will reboot automatically
7. Page will reload after 5 seconds

## 🎨 Visual Feedback

### Drop Zone States

**Normal:**
- Light gray dashed border
- "Drop firmware here or click to select"

**Hover:**
- Purple border
- Light purple background

**Drag Over (Active Drop):**
- Solid purple border
- Glowing purple background
- Pulsing 📦 icon
- Slight scale animation

### Progress Indicators

- **Progress Bar**: Animated gradient fill (0-100%)
- **Status Text**: Real-time upload information
- **Success**: Green checkmark and message
- **Error**: Red X and error message

## 📁 Supported File Types

### Firmware Files (.bin)

**Regular Firmware:**
- File: `firmware.bin` or `firmware_with_ota.bin`
- Type: Application firmware (U_FLASH)
- What it updates: Your device program code

**Filesystem:**
- File: `spiffs.bin` or `littlefs.bin`
- Type: Filesystem image (U_SPIFFS)
- What it updates: Stored files and data

The system automatically detects the file type based on the filename!

## 🔧 Technical Details

### Update Process

1. **Upload**: Firmware is sent via HTTP POST to `/update`
2. **Validation**: ESP32 Update library validates the firmware
3. **Flash**: New firmware is written to flash memory
4. **Verify**: Integrity check ensures correct write
5. **Reboot**: Device restarts with new firmware

### Memory Requirements

- **Firmware Size**: Up to ~1.3MB (depends on partition table)
- **Upload Buffer**: Streamed, no need to store entire file in RAM
- **Progress Updates**: Real-time via XMLHttpRequest

### Security Notes

⚠️ **Important**: The current implementation does not include:
- Firmware signature verification
- Encrypted uploads
- Authentication

**For production use**, consider adding:
- HTTPS (SSL/TLS)
- Firmware signing
- Password protection for OTA endpoint

## 🚨 Troubleshooting

### Upload Fails

**Symptom**: "Upload failed" or "Upload error"

**Solutions**:
1. Check file is a valid `.bin` firmware file
2. Ensure device has enough free flash space
3. Verify WiFi connection is stable
4. Try a smaller firmware (disable features if needed)
5. Reboot device and try again

### Device Doesn't Reboot

**Symptom**: Upload succeeds but device doesn't restart

**Solutions**:
1. Wait 10 seconds, might be delayed
2. Manually power cycle the device
3. Check serial monitor for error messages

### Can't Access Web Interface

**Symptom**: Can't reach http://slap-ai.local

**Solutions**:
1. Verify device is connected to WiFi (check display)
2. Try the IP address instead (check router or use AP mode)
3. Ensure mDNS is supported on your device/network
4. Try from a different browser/device

### "Invalid firmware" Error

**Symptom**: Upload rejected as invalid

**Solutions**:
1. Ensure you're uploading the correct board firmware
2. Check PlatformIO build completed successfully
3. Don't upload a corrupted or incomplete file
4. Rebuild firmware with: `pio run --target clean && pio run`

## 📝 Build & Upload Workflow

### Development Cycle

```powershell
# 1. Make code changes
# (edit your source files)

# 2. Build firmware
pio run

# 3. USB Upload (first time or if OTA fails)
pio run --target upload

# 4. OTA Upload (subsequent updates)
# - Open http://slap-ai.local in browser
# - Drag firmware_with_ota.bin to the web page
# - Wait for reboot
```

### CI/CD Friendly

```powershell
# Build and copy firmware to release directory
pio run
Copy-Item .pio\build\lolin_s3_mini\firmware.bin .\releases\firmware_v1.2.3.bin

# Now you can distribute releases/firmware_v1.2.3.bin
# Users can update via web interface!
```

## 🎯 Best Practices

### Before Updating

1. ✅ Test firmware on one device first
2. ✅ Keep a backup of known-good firmware
3. ✅ Note current configuration settings
4. ✅ Ensure device is on stable power (not battery if low)
5. ✅ Have USB cable ready as backup

### During Update

1. 🚫 Don't close browser tab
2. 🚫 Don't power off device
3. 🚫 Don't disconnect WiFi
4. ✅ Watch progress bar reach 100%
5. ✅ Wait for automatic reboot

### After Update

1. ✅ Verify device boots successfully
2. ✅ Check display shows expected content
3. ✅ Test slap detection
4. ✅ Verify web interface still accessible
5. ✅ Check serial monitor for any errors

## 🎉 Advanced Usage

### OTA via Command Line (Alternative)

You can also use `curl` or similar tools:

```bash
# Upload firmware via curl
curl -F "firmware=@firmware_with_ota.bin" http://slap-ai.local/update

# Check for success response
echo $?
```

### Batch Updates (Multiple Devices)

```powershell
# Update multiple devices
$devices = @("slap-ai-1.local", "slap-ai-2.local", "slap-ai-3.local")
foreach ($device in $devices) {
    curl -F "firmware=@firmware_with_ota.bin" "http://$device/update"
    Write-Host "Updated $device"
    Start-Sleep -Seconds 10
}
```

### Version Tracking

Consider embedding version info in your firmware:

```cpp
// In main.cpp
#define FIRMWARE_VERSION "1.2.3"

void setup() {
    Serial.printf("Slap-AI Firmware v%s\n", FIRMWARE_VERSION);
    // ... rest of setup
}
```

## 📚 Additional Resources

- **ESP32 Update Library**: [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/update.html)
- **OTA Updates**: [ESP-IDF OTA Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/system/ota.html)
- **PlatformIO OTA**: [PlatformIO OTA Documentation](https://docs.platformio.org/en/latest/platforms/espressif32.html#over-the-air-ota-update)

## 🎊 Success!

You now have a professional wireless update system for your Slap-AI device! No more unplugging and replugging USB cables. Just build, drag, drop, and you're done! 🚀

---

**First-time setup**: Use USB to upload the firmware with OTA support, then all future updates can be done wirelessly! ✨

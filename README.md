# WEMOS S3 MINI PRO - IMU Display Test

Hardware test application for the WEMOS S3 MINI PRO that displays real-time IMU sensor data on the built-in TFT screen.

## Hardware Features Used

- **ESP32-S3FH4R2** microcontroller (4MB Flash, 2MB PSRAM)
- **0.85" 128x128 TFT Display** (GC9107/GC9A01 driver)
- **QMI8658C 6-axis IMU** (accelerometer + gyroscope)
- **Serial output** for debugging

## What This Does

This application:
1. Initializes the TFT display and IMU sensor
2. Reads accelerometer data (X, Y, Z in g-force)
3. Reads gyroscope data (X, Y, Z in degrees/second)
4. Displays the data on the screen in real-time (20Hz refresh rate)
5. Outputs data to Serial for debugging

## Pin Configuration

See [docs/pinout.md](docs/pinout.md) for complete pin mapping.

### Key Pins:
- **I2C (IMU)**: SDA=GPIO6, SCL=GPIO5
- **SPI (TFT)**: MOSI=GPIO11, SCLK=GPIO12, CS=GPIO4, DC=GPIO2, RST=GPIO1
- **Backlight**: GPIO3
- **LED**: Power=GPIO7, Data=GPIO8

## Prerequisites

1. **PlatformIO** - Install via VS Code extension or CLI
2. **USB Cable** - USB-C cable to connect the board

## Building and Uploading

### Using VS Code with PlatformIO:

1. Open this folder in VS Code
2. PlatformIO should auto-detect the project
3. Click the checkmark (✓) icon to build
4. Connect your WEMOS S3 MINI PRO via USB
5. Click the arrow (→) icon to upload

### Using PlatformIO CLI:

```bash
# Build the project
pio run

# Upload to board
pio run --target upload

# Open serial monitor
pio device monitor
```

## Expected Behavior

1. **Startup Screen**: Shows "WEMOS S3 IMU Test" and initialization status
2. **Main Display**: Shows real-time data:
   - Accelerometer readings in g (gravity units)
   - Gyroscope readings in degrees/second
   - Temperature in Celsius

3. **Serial Output**: Continuous data stream at 115200 baud

## Testing the IMU

- **Accelerometer**: Tilt the board in different directions. Z-axis should show ~1.0g when flat
- **Gyroscope**: Rotate the board. Values should change during movement and return to ~0 when still
- **Temperature**: Should show the chip temperature

## Troubleshooting

### "IMU FAIL!" displayed on screen
- Check I2C connections
- Verify the IMU I2C address (try 0x6A if 0x6B doesn't work)
- Check soldering on IMU sensor

### Display not working
- Check TFT_eSPI configuration in `include/User_Setup.h`
- Verify pin connections match the schematic
- Check backlight is enabled (GPIO3)

### Upload fails / MicroPython shows up
⚠️ **The board comes with MicroPython pre-installed!**

**Solution**: Hold the **BOOT button (GPIO0)** when you see "Connecting..." during upload.

See detailed instructions: [MicroPython Troubleshooting Guide](docs/micropython_troubleshooting.md)

Quick fix:
```powershell
# Start upload
pio run --target upload

# When you see "Connecting...", HOLD BOOT button for 2-3 seconds
```

Or erase flash first:
```powershell
pio run --target erase  # Hold BOOT button
pio run --target upload # Hold BOOT button again
```

### Upload fails (other reasons)
- Try different USB cable
- Check USB drivers for ESP32-S3
- Verify COM port in Device Manager

## Customization

### Change Display Refresh Rate
Edit `updateInterval` in `src/main.cpp`:
```cpp
const unsigned long updateInterval = 50; // milliseconds (20Hz)
```

### Change IMU Sensitivity
Edit scaling factors in `include/QMI8658C.h`:
```cpp
float accelScale = 2.0 / 32768.0;  // ±2g range
float gyroScale = 250.0 / 32768.0; // ±250 dps range
```

### Modify Display Layout
Edit the display code in `src/main.cpp` loop() function

## References

- [WEMOS S3 MINI PRO Documentation](https://www.wemos.cc/en/latest/s3/s3_mini_pro.html)
- [Schematic PDF](https://www.wemos.cc/en/latest/_static/files/sch_s3_mini_pro_v1.0.0.pdf)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)

## License

See LICENSE file in the root directory.

## Next Steps

Potential enhancements:
- Add visual graphs for IMU data
- Implement motion detection
- Add button controls
- WiFi connectivity for remote monitoring
- Data logging to SD card or flash
- RGB LED status indicators



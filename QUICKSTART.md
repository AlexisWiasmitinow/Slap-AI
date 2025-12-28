# Quick Start Guide

## 🚀 Get Running in 3 Steps

### 1. Install PlatformIO
If you haven't already:
- **VS Code**: Install the "PlatformIO IDE" extension
- **CLI**: `pip install platformio`

### 2. Build the Project
```bash
cd C:\Code\Slap-AI
pio run
```

Or in VS Code: Click the checkmark ✓ icon in the bottom toolbar

### 3. Upload to Board

⚠️ **IMPORTANT**: The board has MicroPython pre-installed!

```bash
# Start the upload
pio run --target upload

# When you see "Connecting...", immediately HOLD the BOOT button (GPIO0)
# Keep holding for 2-3 seconds until upload starts
```

Or in VS Code: 
1. Click the arrow → icon
2. When you see "Connecting...", **HOLD BOOT button**

**Alternative**: Erase flash first (removes MicroPython):
```bash
.\build.ps1
# Choose option 6, hold BOOT button
# Then choose option 2 to upload
```

📖 **Having trouble?** See [MicroPython Troubleshooting Guide](docs/micropython_troubleshooting.md)

## 📊 What You'll See

**On the TFT Screen:**
```
=== IMU DATA ===
Accel (g):
X:  0.123
Y: -0.045
Z:  1.002

Gyro (d/s):
X:   1.23
Y:  -2.45
Z:   0.12

T: 28.5C
```

**In Serial Monitor (115200 baud):**
```
Accel: X=0.123 Y=-0.045 Z=1.002 | Gyro: X=1.23 Y=-2.45 Z=0.12 | Temp=28.5°C
```

## 🔧 Hardware Connections

The board has everything built-in! Just:
1. Connect via USB-C
2. Wait for drivers to install (Windows will auto-install)
3. Upload the code

## ✅ Testing

Move the board around and watch:
- **Accelerometer** values change with tilt
- **Gyroscope** values change with rotation
- **Z-axis** should be ~1.0g when board is flat

## 🐛 Common Issues

| Problem | Solution |
|---------|----------|
| Can't upload | Hold BOOT button (GPIO0) during upload |
| "IMU FAIL" | Check I2C address (try 0x6A or 0x6B) |
| Blank screen | Check TFT backlight is on (GPIO3) |
| No serial output | Set baud rate to 115200 |

## 📁 Project Structure

```
Slap-AI/
├── platformio.ini          # Project configuration
├── src/
│   ├── main.cpp           # Main application
│   └── QMI8658C.cpp       # IMU driver implementation
├── include/
│   ├── QMI8658C.h         # IMU driver header
│   └── User_Setup.h       # TFT display configuration
└── docs/
    └── pinout.md          # Pin mapping reference
```

## 🎯 Next Steps

1. Try tilting the board - see accelerometer values change
2. Rotate the board - see gyroscope values change
3. Modify refresh rate in `main.cpp`
4. Add custom display elements
5. Implement motion detection algorithms

---

**Need help?** Check the full [README.md](README.md) for detailed information.



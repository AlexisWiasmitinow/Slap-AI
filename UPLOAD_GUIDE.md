# 🚀 QUICK UPLOAD GUIDE

## The Problem
Your board has **MicroPython** pre-installed. You need to enter **bootloader mode** to upload Arduino firmware.

---

## ⚡ FASTEST SOLUTION

### Step 1: Start Upload
```powershell
pio run --target upload
```

### Step 2: Watch for "Connecting..."
```
Connecting........___
```

### Step 3: HOLD BOOT BUTTON NOW! 
**↓ Press and hold this button ↓**

```
┌─────────────┐
│ WEMOS S3    │
│             │
│ ┌─────────┐ │
│ │ [BOOT]  │ ← HOLD THIS!
│ │ [IO47]  │ │
│ │ [IO48]  │ │
│ └─────────┘ │
│             │
│  [Screen]   │
└─────────────┘
```

### Step 4: Keep Holding (2-3 seconds)
Wait until you see:
```
Chip is ESP32-S3
```

### Step 5: Release & Done! ✅
Upload continues automatically.

---

## 🔧 ALTERNATIVE: Erase First

If the above doesn't work:

```powershell
# 1. Erase flash (HOLD BOOT when you see "Connecting...")
pio run --target erase

# 2. Upload (HOLD BOOT again)
pio run --target upload
```

---

## 💡 TIPS

✅ **DO**: Hold BOOT button when "Connecting..." appears  
✅ **DO**: Use a good quality USB cable  
✅ **DO**: Close any serial monitors first  

❌ **DON'T**: Hold BOOT too early  
❌ **DON'T**: Release BOOT too quickly  
❌ **DON'T**: Use a power-only USB cable  

---

## 🎯 SUCCESS INDICATORS

### ✅ Upload Started:
```
Chip is ESP32-S3 (revision v0.2)
Features: WiFi, BLE
Crystal is 40MHz
Uploading stub...
```

### ✅ Upload Complete:
```
Hard resetting via RTS pin...
```

### ✅ On TFT Screen:
```
WEMOS S3
IMU Test
Init IMU...
IMU OK!
```

---

## 📚 More Help

- [Full Troubleshooting Guide](docs/micropython_troubleshooting.md)
- [Complete README](README.md)
- [Quick Start](QUICKSTART.md)

---

**Remember**: MicroPython is not a problem - just hold BOOT button during upload! 🎉


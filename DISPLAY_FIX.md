# Quick Fix: Display is Off

## ⚡ **Immediate Actions:**

### 1. **Close Serial Monitor**
If you have serial monitor open, **close it first** before uploading:
- Press `Ctrl+C` in the monitor terminal
- Or close the terminal window

### 2. **Upload Updated Code**
```powershell
pio run --target upload
# Remember: HOLD BOOT button when you see "Connecting..."
```

### 3. **Open Serial Monitor (After Upload)**
```powershell
pio device monitor
```

### 4. **Check Serial Output**
You should see:
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
```

---

## 🔍 **What to Look For:**

### **Scenario A: You see a RED FLASH on the display**
✅ **Display works!** The code is running.  
The display should then show IMU data.

### **Scenario B: Display stays dark, but serial shows messages**
Try **inverted backlight**:

Edit `src/main.cpp`, line ~28:
```cpp
digitalWrite(TFT_BL, LOW); // Change HIGH to LOW
```

Rebuild and upload.

### **Scenario C: No serial output at all**
The code might be crashing. Try the simple display test:

1. **Backup main.cpp:**
   ```powershell
   Copy-Item src/main.cpp src/main.cpp.backup
   ```

2. **Use test file:**
   ```powershell
   Copy-Item test/display_test.cpp src/main.cpp
   ```

3. **Upload:**
   ```powershell
   pio run --target upload
   ```

4. **Monitor:**
   ```powershell
   pio device monitor
   ```

The test will cycle RED → GREEN → BLUE colors and tell you in serial what to expect.

### **Scenario D: Serial shows errors**
Copy the error and let me know!

---

## 🎯 **Most Likely Fixes:**

| Issue | Fix | File to Edit |
|-------|-----|--------------|
| Backlight off | Change `HIGH` to `LOW` | `src/main.cpp` line ~28 |
| Wrong driver | Change driver in header | `include/User_Setup.h` |
| Pin mismatch | Verify pins match board | `include/User_Setup.h` |

---

## 📝 **What's Changed:**

The updated code now:
- ✅ Has more serial debug output
- ✅ Shows a **RED flash** on startup (test if display works)
- ✅ Longer delays for stability
- ✅ Better error reporting

---

## 🆘 **Need Help?**

Tell me:
1. **What do you see on serial monitor?** (copy/paste output)
2. **Does the display show anything?** (even a brief flash)
3. **Does the backlight turn on?** (display glows even if black)

See full guide: [docs/display_troubleshooting.md](docs/display_troubleshooting.md)


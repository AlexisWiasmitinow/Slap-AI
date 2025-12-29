# Slap-AI WiFi Setup Guide

## ✅ System Overview

Your WEMOS S3 MINI PRO is now a fully-featured slap detector with WiFi configuration capabilities!

## 🚀 Features Implemented

### 1. **Motion Detection**
- Configurable slap threshold (0.1g - 5.0g)
- Default threshold: 1.0g
- Red screen with "SLAP!" message on detection
- 3-second display timeout
- Automatic return to normal state

### 2. **WiFi Management**
- **AP Mode (Default)**
  - SSID: `slap-ai` (open network, no password)
  - IP Address: **10.0.0.1**
  - Creates its own WiFi hotspot
  
- **Client Mode**
  - Connects to your home WiFi
  - Display shows connection status
  - Auto-reverts to AP if connection fails

### 3. **Web Interface**
- Modern, responsive web UI
- Real-time status updates
- Threshold adjustment slider
- WiFi configuration
- Factory reset button

### 4. **Factory Reset**
- Hold IO47 button for 5 seconds
- Visual progress bar on display
- Resets to AP mode with default settings

## 📱 How to Use

### First Time Setup

1. **Power on the device**
   - Display will show "AP MODE" with IP address

2. **Connect to WiFi**
   - On your phone/computer, find WiFi network: **`slap-ai`**
   - Connect (no password needed)

3. **Open Web Interface**
   - Open browser and go to: **http://10.0.0.1**
   - You should see the Slap-AI settings page

4. **Configure WiFi (Optional)**
   - Enter your home WiFi name and password
   - Click "Connect to WiFi"
   - Device will restart and connect to your network

5. **Adjust Threshold (Optional)**
   - Use the slider to set detection sensitivity
   - Default 1.0g works well for hand slaps
   - Higher values = need harder slap
   - Lower values = more sensitive
   - Click "Save Threshold"

### Normal Operation

**In AP Mode:**
- Display shows: "AP MODE" + IP address
- Access web interface at: http://10.0.0.1
- Slap detection is active

**In Client Mode (Connected to WiFi):**
- Display is black (idle)
- Slap detection is active
- When slap detected: Red screen with "SLAP!" for 3 seconds
- **Access web interface at: http://slap-ai.local** ✨
- Alternative: Find device IP on your router, or reset to AP mode

### Factory Reset

- Press and hold the button on **IO47** for 5 seconds
- Display shows progress bar
- Device resets to AP mode with default settings

## 🎯 Display States

| State | Display |
|-------|---------|
| **AP Mode** | "AP MODE" + "slap-ai (open)" + IP address |
| **Connecting** | "Connecting to [WiFi Name]..." with animated dots |
| **Connected** | Black screen (ready for slap detection) |
| **Slap Detected** | Red screen with large "SLAP!" text (3 seconds) |
| **Resetting** | "RESET" with progress bar |

## 🔧 Web Interface Features

### Status Panel
- Current mode (AP/Client)
- IP address
- Signal strength (when connected)

### Threshold Setting
- Visual slider (0.1g to 5.0g)
- Real-time value display
- Save button

### WiFi Configuration
- SSID input field
- Password input field (for home WiFi)
- Connect button
- Device restarts after saving

### Factory Reset
- Confirmation dialog
- Resets all settings to defaults
- Returns to AP mode

## 📊 Configuration Storage

All settings are saved in non-volatile memory (NVS):
- WiFi mode (AP/Client)
- WiFi credentials
- Slap threshold
- Settings persist through power cycles

## 🛠️ Technical Details

### Network Information

**AP Mode:**
- IP: 10.0.0.1
- Gateway: 10.0.0.1
- Subnet: 255.255.255.0
- No DHCP range needed (single client typically)
- Access: http://10.0.0.1

**Client Mode:**
- IP assigned by your router (DHCP)
- **mDNS hostname: slap-ai.local** ✨
- Access: **http://slap-ai.local** (recommended)
- Alternative: Find IP on your router or use network scanner app

### API Endpoints

The web interface uses these REST API endpoints:

- `GET /` - Main web interface
- `GET /api/status` - Current status (JSON)
- `POST /api/threshold` - Set slap threshold
- `POST /api/wifi` - Configure WiFi credentials
- `POST /api/reset` - Factory reset

### Button Specifications

- Pin: IO47
- Type: Pull-up (active LOW)
- Long press: 5 seconds
- Visual feedback: Progress bar on display

## 🐛 Troubleshooting

### Can't connect to `slap-ai` WiFi
- Make sure device is powered on
- Check display shows "AP MODE"
- WiFi is open (no password)
- Try forgetting the network and reconnecting

### Can't access http://10.0.0.1
- Make sure you're connected to `slap-ai` WiFi
- Try http://10.0.0.1 (not https)
- Clear browser cache
- Try a different browser

### Device won't connect to home WiFi
- Check WiFi credentials are correct
- Make sure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Device will revert to AP mode after 20 seconds if connection fails

### Slap detection too sensitive/not sensitive enough
- Adjust threshold via web interface
- Range: 0.1g (very sensitive) to 5.0g (need hard slap)
- Recommended starting point: 1.0g

### Need to reset but button doesn't work
- Hold button for full 5 seconds
- Watch for progress bar on display
- Button is on IO47

## 🎉 Success Indicators

✅ **AP Mode Working:** Display shows "AP MODE" and IP address  
✅ **WiFi Connecting:** Display shows "Connecting to..." with dots  
✅ **WiFi Connected:** Display goes black + accessible at http://slap-ai.local  
✅ **Slap Detection:** Red screen with "SLAP!" appears when you slap the device  
✅ **Web Interface (AP):** Can access settings at http://10.0.0.1  
✅ **Web Interface (Client):** Can access settings at http://slap-ai.local  
✅ **Settings Saved:** Configuration persists after power cycle  

## 📝 Next Steps

1. Test slap detection with different thresholds
2. Connect to your home WiFi
3. Find a good mounting location
4. Enjoy your motion-triggered display!

---

**Need help?** Check the serial monitor output for detailed debug information:
```bash
pio device monitor
```


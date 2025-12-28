# WEMOS S3 MINI PRO Hardware Pin Mapping

## Pin Definitions Based on Schematic

### ⚠️ CRITICAL: SHARED PINS!
**GPIO 11 and GPIO 12 are SHARED between TFT and IMU!**
- GPIO 11: TFT_SCLK (SPI) / IMU_SCL (I2C)  
- GPIO 12: TFT_MOSI (SPI) / IMU_SDA (I2C)

These must be time-multiplexed (switch between I2C and SPI modes).

### TFT Display (GC9107/GC9A01) - SPI Interface
- **TFT_CS**: GPIO 4
- **TFT_DC**: GPIO 2 (Data/Command)
- **TFT_RST**: GPIO 1 (Reset)
- **TFT_MOSI**: GPIO 12 ⚠️ SHARED with IMU_SDA
- **TFT_SCLK**: GPIO 11 ⚠️ SHARED with IMU_SCL
- **TFT_BL**: GPIO 3 (Backlight)
- **Display Size**: 128x128 pixels
- **Driver**: GC9107 or GC9A01

### IMU Sensor (QMI8658C) - I2C Interface
- **IMU_SDA**: GPIO 12 ⚠️ SHARED with TFT_MOSI
- **IMU_SCL**: GPIO 11 ⚠️ SHARED with TFT_SCLK
- **IMU_INT1**: GPIO 18 (Interrupt 1)
- **IMU_INT2**: GPIO 21 (Interrupt 2)
- **I2C Address**: 0x6B

### RGB LED (WS2812 or similar)
- **LED_DATA**: GPIO 8
- **LED_POWER**: GPIO 7

### Buttons
- **BTN_0**: GPIO 0 (Boot button)
- **BTN_47**: GPIO 47
- **BTN_48**: GPIO 48

### IR Transmitter
- **IR_TX**: GPIO 9

### Available I/O Pins
Additional exposed pins: Check schematic for specific requirements

## Notes
- All pins operate at 3.3V
- ESP32-S3 has 4MB Flash and 2MB PSRAM
- Default USB CDC enabled for serial communication



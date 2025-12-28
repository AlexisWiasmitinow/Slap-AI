# Hardware Technical Notes

## WEMOS S3 MINI PRO Specifications

### Microcontroller: ESP32-S3FH4R2
- **CPU**: Xtensa dual-core 32-bit LX7 @ 240MHz
- **Flash**: 4MB
- **PSRAM**: 2MB
- **WiFi**: 802.11 b/g/n (2.4GHz)
- **Bluetooth**: BLE 5.0
- **USB**: Native USB OTG support

### Display: 0.85" TFT LCD
- **Resolution**: 128x128 pixels
- **Driver IC**: GC9107 or GC9A01 (compatible)
- **Interface**: SPI
- **Colors**: 65K (RGB565)
- **Viewing Angle**: Good (IPS-like)

### IMU: QMI8658C
- **Type**: 6-axis MEMS (3-axis accelerometer + 3-axis gyroscope)
- **Interface**: I2C
- **I2C Address**: 0x6B (sometimes 0x6A)
- **Accelerometer Ranges**: ±2g, ±4g, ±8g, ±16g
- **Gyroscope Ranges**: ±16, ±32, ±64, ±128, ±256, ±512, ±1024, ±2048 dps
- **Output Data Rate**: Up to 1000Hz
- **Features**: Built-in FIFO, interrupt support

## Pin Mapping Details

### SPI Bus (Display)
```
ESP32-S3    →    TFT Display
GPIO 11     →    MOSI (Data)
GPIO 12     →    SCK (Clock)
GPIO 4      →    CS (Chip Select)
GPIO 2      →    DC (Data/Command)
GPIO 1      →    RST (Reset)
GPIO 3      →    BL (Backlight)
```

### I2C Bus (IMU)
```
ESP32-S3    →    QMI8658C
GPIO 6      →    SDA (Data)
GPIO 5      →    SCL (Clock)
GPIO 10     →    INT (Interrupt, optional)
```

### Other Peripherals
```
GPIO 7      →    RGB LED Power
GPIO 8      →    RGB LED Data (WS2812)
GPIO 9      →    IR Transmitter
GPIO 0      →    BOOT Button
GPIO 47     →    User Button 1
GPIO 48     →    User Button 2
```

## Power Specifications

- **Operating Voltage**: 3.3V (regulated from USB 5V)
- **Typical Current**: 
  - Active (WiFi off): ~80mA
  - Active (WiFi on): ~150-250mA
  - Deep sleep: <1mA
- **USB Power**: 5V via USB-C
- **Battery**: Can be powered via BAT pin (3.7V LiPo)

## Communication Protocols

### I2C Configuration
```cpp
Wire.begin(SDA_PIN, SCL_PIN);
Wire.setClock(400000);  // 400kHz Fast Mode
```

### SPI Configuration
```cpp
// Handled by TFT_eSPI library
// Max frequency: 40MHz (tested stable)
```

## Memory Layout

- **Code/ROM**: Uses flash memory (XIP - Execute In Place)
- **RAM**: 
  - 512KB SRAM
  - 2MB PSRAM (for large buffers/graphics)
- **Stack**: Typically 8KB per task
- **Heap**: Remaining RAM after static allocation

## Thermal Considerations

- **Operating Temperature**: -40°C to +85°C
- **Typical Operating Temp**: 30-45°C (depends on load)
- **IMU Temperature Sensor**: Measures chip temperature (not ambient)

## Programming Notes

### USB CDC (Serial)
The board uses native USB CDC, so Serial output appears as a virtual COM port. No external USB-to-serial chip needed.

### Boot Mode
- **Normal Boot**: Release GPIO0 during startup
- **Download Mode**: Hold GPIO0 low during startup (for firmware upload)

### PSRAM Usage
PSRAM is enabled and can be used for:
- Large graphics buffers
- WiFi/BLE buffers
- Data logging arrays

## Known Issues & Workarounds

### 1. Display Driver Variants
Some boards use GC9107, others use GC9A01. Both are compatible with the GC9A01 driver in TFT_eSPI.

### 2. IMU I2C Address
If IMU initialization fails with address 0x6B, try 0x6A:
```cpp
#define QMI8658C_I2C_ADDR 0x6A  // Alternative address
```

### 3. USB Serial on Startup
ESP32-S3 requires USB CDC to be enabled for Serial output:
```ini
-DARDUINO_USB_CDC_ON_BOOT=1
```

## Performance Optimization

### Display Refresh Rate
- **Current**: 20Hz (50ms interval)
- **Maximum**: ~60Hz (depends on SPI speed)
- **Recommended**: 20-30Hz for good balance

### IMU Sampling Rate
- **Current**: 250Hz ODR
- **Maximum**: 1000Hz
- **Note**: Faster sampling = more power consumption

### Power Saving
```cpp
// Reduce display brightness
analogWrite(TFT_BL, 128);  // 50% brightness

// Reduce IMU sample rate
// Modify CTRL2 and CTRL3 register values

// Use WiFi only when needed
WiFi.mode(WIFI_OFF);
```

## Schematic References

- Full schematic: [sch_s3_mini_pro_v1.0.0.pdf](https://www.wemos.cc/en/latest/_static/files/sch_s3_mini_pro_v1.0.0.pdf)
- Dimensions: 34.3mm × 25.4mm
- Weight: 5g

## Development Tools

### Recommended
- **PlatformIO**: Best for Arduino framework development
- **ESP-IDF**: For advanced features and optimization
- **Arduino IDE**: Works but PlatformIO is preferred

### Debugging
- **USB CDC Serial**: Built-in (115200 baud)
- **JTAG**: Available on specific pins (see schematic)
- **Logging**: ESP32 log levels configurable

## Expansion Options

The board exposes additional pins that can be used for:
- Additional I2C devices (same bus as IMU)
- Additional SPI devices (same bus as display)
- UART communication
- ADC inputs (analog sensors)
- DAC outputs
- PWM outputs (LED control, servo, etc.)

Refer to [pinout.md](pinout.md) for available GPIO pins.



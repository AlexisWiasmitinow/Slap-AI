// TFT_eSPI User Setup for WEMOS S3 MINI PRO
// 0.85" 128x128 TFT Display with GC9107 driver

#ifndef USER_SETUP_H
#define USER_SETUP_H

// Define the driver
// The board may use GC9107 or GC9A01 - they're similar but try GC9A01 first
#define GC9A01_DRIVER  // GC9A01 driver is compatible with GC9107

// If display doesn't work, comment above and uncomment below:
// #define ST7735_DRIVER
// #define CGRAM_OFFSET

// Display size
#define TFT_WIDTH  128
#define TFT_HEIGHT 128

// Pin definitions for WEMOS S3 MINI PRO
// Based on schematic: https://www.wemos.cc/en/latest/_static/files/sch_s3_mini_pro_v1.0.0.pdf
#define TFT_MOSI 11  // SPI MOSI
#define TFT_SCLK 12  // SPI Clock  
#define TFT_CS   4   // Chip Select
#define TFT_DC   2   // Data/Command
#define TFT_RST  1   // Reset
// Note: TFT_BL (backlight) is controlled manually in code

// SPI frequency - try lower if issues
#define SPI_FREQUENCY  27000000  // 27 MHz (reduced from 40MHz for stability)
#define SPI_READ_FREQUENCY  16000000
#define SPI_TOUCH_FREQUENCY  2500000

// Color order (might need adjustment)
// #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
// #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Display rotation (0-3, adjust if needed)
#define TFT_ROTATION 0

// Backlight control - set to -1 if not used
#define TFT_BL 3

// Font settings
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH
#define LOAD_FONT8  // Font 8. Large 75 pixel font, needs ~3256 bytes in FLASH

#define SMOOTH_FONT

#endif // USER_SETUP_H



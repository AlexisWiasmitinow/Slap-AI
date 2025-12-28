// Minimal TFT_eSPI setup for ESP32-S3 WEMOS S3 MINI PRO
// Testing configuration

#define USER_SETUP_INFO "User_Setup"

// Driver
#define GC9A01_DRIVER

// Display
#define TFT_WIDTH 128
#define TFT_HEIGHT 128

// Pins according to WEMOS S3 MINI PRO schematic
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   4
#define TFT_DC   2
#define TFT_RST  1

// Frequencies
#define SPI_FREQUENCY 20000000
#define SPI_READ_FREQUENCY 10000000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define SMOOTH_FONT


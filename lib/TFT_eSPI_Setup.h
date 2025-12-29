// TFT_eSPI setup for ESP32-S3 WEMOS S3 MINI PRO
// Correct pins from schematic (2024-06-21)

#define USER_SETUP_INFO "User_Setup"

// Driver
#define GC9A01_DRIVER

// Display
#define TFT_WIDTH 128
#define TFT_HEIGHT 128

// Pins from schematic - MCU page
#define TFT_MOSI 38  // IO38/MOSI
#define TFT_SCLK 40  // IO40/SCK
#define TFT_CS   35  // TFT_CS/IO35
#define TFT_DC   36  // TFT_DC/IO36
#define TFT_RST  34  // TFT_RST/IO34

// Frequencies
#define SPI_FREQUENCY 27000000
#define SPI_READ_FREQUENCY 16000000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define SMOOTH_FONT


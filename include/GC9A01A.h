/*
 * GC9A01A Display Driver for Adafruit GFX
 * Minimal driver for 128x128 round display
 */

#ifndef GC9A01A_H
#define GC9A01A_H

#include <Adafruit_GFX.h>
#include <SPI.h>

// GC9A01A Commands
#define GC9A01A_SLPOUT 0x11
#define GC9A01A_DISPON 0x29
#define GC9A01A_CASET  0x2A
#define GC9A01A_RASET  0x2B
#define GC9A01A_RAMWR  0x2C
#define GC9A01A_MADCTL 0x36
#define GC9A01A_COLMOD 0x3A

class GC9A01A : public Adafruit_GFX {
public:
    GC9A01A(int8_t cs, int8_t dc, int8_t rst);
    
    void begin(uint32_t freq = 27000000);
    void setRotation(uint8_t r);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillScreen(uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
private:
    SPIClass *_spi;
    int8_t _cs, _dc, _rst;
    
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
};

#endif


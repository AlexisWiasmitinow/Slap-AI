/*
 * GC9A01A Display Driver Implementation
 */

#include "GC9A01A.h"

GC9A01A::GC9A01A(int8_t cs, int8_t dc, int8_t rst) 
    : Adafruit_GFX(128, 128), _cs(cs), _dc(dc), _rst(rst) {
    _spi = &SPI;
}

void GC9A01A::begin(uint32_t freq) {
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_rst, OUTPUT);
    
    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);
    
    // Reset display
    digitalWrite(_rst, HIGH);
    delay(10);
    digitalWrite(_rst, LOW);
    delay(10);
    digitalWrite(_rst, HIGH);
    delay(120);
    
    // Initialize SPI
    _spi->begin(40, -1, 38, _cs);  // SCK, MISO, MOSI, CS
    _spi->beginTransaction(SPISettings(freq, MSBFIRST, SPI_MODE0));
    
    // Init sequence
    writeCommand(0xEF);
    writeCommand(0xEB); writeData(0x14);
    
    writeCommand(0xFE); writeCommand(0xEF);
    writeCommand(0xEB); writeData(0x14);
    writeCommand(0x84); writeData(0x40);
    writeCommand(0x85); writeData(0xFF);
    writeCommand(0x86); writeData(0xFF);
    writeCommand(0x87); writeData(0xFF);
    writeCommand(0x88); writeData(0x0A);
    writeCommand(0x89); writeData(0x21);
    writeCommand(0x8A); writeData(0x00);
    writeCommand(0x8B); writeData(0x80);
    writeCommand(0x8C); writeData(0x01);
    writeCommand(0x8D); writeData(0x01);
    writeCommand(0x8E); writeData(0xFF);
    writeCommand(0x8F); writeData(0xFF);
    
    writeCommand(0xB6); writeData(0x00); writeData(0x00);
    
    writeCommand(GC9A01A_MADCTL); writeData(0x68);  // Changed from 0x48 to 0x68 to fix mirroring
    writeCommand(GC9A01A_COLMOD); writeData(0x05);  // 16-bit color
    
    writeCommand(GC9A01A_SLPOUT);
    delay(120);
    
    writeCommand(GC9A01A_DISPON);
    delay(20);
    
    _spi->endTransaction();
}

void GC9A01A::writeCommand(uint8_t cmd) {
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    _spi->transfer(cmd);
    digitalWrite(_cs, HIGH);
}

void GC9A01A::writeData(uint8_t data) {
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    _spi->transfer(data);
    digitalWrite(_cs, HIGH);
}

void GC9A01A::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t x2 = x + w - 1;
    uint16_t y2 = y + h - 1;
    
    writeCommand(GC9A01A_CASET);
    writeData(x >> 8); writeData(x & 0xFF);
    writeData(x2 >> 8); writeData(x2 & 0xFF);
    
    writeCommand(GC9A01A_RASET);
    writeData(y >> 8); writeData(y & 0xFF);
    writeData(y2 >> 8); writeData(y2 & 0xFF);
    
    writeCommand(GC9A01A_RAMWR);
}

void GC9A01A::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    
    _spi->beginTransaction(SPISettings(27000000, MSBFIRST, SPI_MODE0));
    setAddrWindow(x, y, 1, 1);
    
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    _spi->transfer16(color);
    digitalWrite(_cs, HIGH);
    
    _spi->endTransaction();
}

void GC9A01A::fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

void GC9A01A::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width)  w = _width  - x;
    if ((y + h - 1) >= _height) h = _height - y;
    
    _spi->beginTransaction(SPISettings(27000000, MSBFIRST, SPI_MODE0));
    setAddrWindow(x, y, w, h);
    
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        _spi->transfer16(color);
    }
    
    digitalWrite(_cs, HIGH);
    _spi->endTransaction();
}

void GC9A01A::setRotation(uint8_t r) {
    rotation = r % 4;
    _spi->beginTransaction(SPISettings(27000000, MSBFIRST, SPI_MODE0));
    writeCommand(GC9A01A_MADCTL);
    switch (rotation) {
        case 0: writeData(0x68); break;  // Fixed mirroring
        case 1: writeData(0xC8); break;
        case 2: writeData(0xA8); break;
        case 3: writeData(0x08); break;
    }
    _spi->endTransaction();
}


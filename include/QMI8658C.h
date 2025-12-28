/*
 * QMI8658C 6-axis IMU Driver
 * 
 * Supports accelerometer and gyroscope reading
 * Communication via I2C
 * 
 * NOTE: On WEMOS S3 MINI PRO, I2C pins are SHARED with TFT SPI!
 * SDA = GPIO 12 (also TFT MOSI)
 * SCL = GPIO 11 (also TFT SCLK)
 */

#ifndef QMI8658C_H
#define QMI8658C_H

#include <Arduino.h>
#include <Wire.h>

// I2C Address
#define QMI8658C_I2C_ADDR 0x6B

// Register addresses
#define QMI8658C_WHO_AM_I     0x00
#define QMI8658C_CTRL1        0x02
#define QMI8658C_CTRL2        0x03
#define QMI8658C_CTRL3        0x04
#define QMI8658C_CTRL4        0x05
#define QMI8658C_CTRL5        0x06
#define QMI8658C_CTRL7        0x08

// Data registers
#define QMI8658C_TEMP_L       0x33
#define QMI8658C_TEMP_H       0x34
#define QMI8658C_AX_L         0x35
#define QMI8658C_AX_H         0x36
#define QMI8658C_AY_L         0x37
#define QMI8658C_AY_H         0x38
#define QMI8658C_AZ_L         0x39
#define QMI8658C_AZ_H         0x3A
#define QMI8658C_GX_L         0x3B
#define QMI8658C_GX_H         0x3C
#define QMI8658C_GY_L         0x3D
#define QMI8658C_GY_H         0x3E
#define QMI8658C_GZ_L         0x3F
#define QMI8658C_GZ_H         0x40

// Expected WHO_AM_I value
#define QMI8658C_CHIP_ID      0x05

struct IMUData {
    float accelX, accelY, accelZ;  // in g
    float gyroX, gyroY, gyroZ;     // in deg/s
    float temperature;              // in celsius
};

class QMI8658C {
public:
    QMI8658C();
    
    bool begin(TwoWire &wire = Wire, uint8_t addr = QMI8658C_I2C_ADDR);
    bool isConnected();
    
    void update();
    IMUData getData();
    
    // Individual data access
    float getAccelX() { return data.accelX; }
    float getAccelY() { return data.accelY; }
    float getAccelZ() { return data.accelZ; }
    float getGyroX() { return data.gyroX; }
    float getGyroY() { return data.gyroY; }
    float getGyroZ() { return data.gyroZ; }
    float getTemperature() { return data.temperature; }
    
private:
    TwoWire *_wire;
    uint8_t _addr;
    IMUData data;
    
    // Calibration factors (can be adjusted)
    float accelScale = 2.0 / 32768.0;  // ±2g range
    float gyroScale = 250.0 / 32768.0; // ±250 dps range
    
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
    int16_t readInt16(uint8_t regLow);
};

#endif // QMI8658C_H



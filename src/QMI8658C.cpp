/*
 * QMI8658C 6-axis IMU Driver Implementation
 */

#include "QMI8658C.h"

QMI8658C::QMI8658C() {
    _wire = nullptr;
    _addr = QMI8658C_I2C_ADDR;
}

bool QMI8658C::begin(TwoWire &wire, uint8_t addr) {
    _wire = &wire;
    _addr = addr;
    
    // Check WHO_AM_I
    uint8_t chipId = readRegister(QMI8658C_WHO_AM_I);
    if (chipId != QMI8658C_CHIP_ID) {
        Serial.printf("QMI8658C: Wrong chip ID: 0x%02X (expected 0x%02X)\n", chipId, QMI8658C_CHIP_ID);
        return false;
    }
    
    Serial.println("QMI8658C: Chip ID verified!");
    
    // Reset and configure the sensor
    delay(10);
    
    // CTRL1: Serial Interface and Sensor Enable
    writeRegister(QMI8658C_CTRL1, 0x40); // Enable sensors
    delay(10);
    
    // CTRL2: Accelerometer settings
    // ODR = 250Hz, Range = ±2g
    writeRegister(QMI8658C_CTRL2, 0x15);
    
    // CTRL3: Gyroscope settings
    // ODR = 250Hz, Range = ±250dps
    writeRegister(QMI8658C_CTRL3, 0x55);
    
    // CTRL7: Enable accelerometer and gyroscope
    writeRegister(QMI8658C_CTRL7, 0x03);
    
    delay(100); // Wait for sensor to stabilize
    
    Serial.println("QMI8658C: Initialized successfully!");
    return true;
}

bool QMI8658C::isConnected() {
    if (_wire == nullptr) return false;
    uint8_t chipId = readRegister(QMI8658C_WHO_AM_I);
    return (chipId == QMI8658C_CHIP_ID);
}

void QMI8658C::update() {
    if (_wire == nullptr) return;
    
    // Read accelerometer data
    int16_t ax = readInt16(QMI8658C_AX_L);
    int16_t ay = readInt16(QMI8658C_AY_L);
    int16_t az = readInt16(QMI8658C_AZ_L);
    
    // Read gyroscope data
    int16_t gx = readInt16(QMI8658C_GX_L);
    int16_t gy = readInt16(QMI8658C_GY_L);
    int16_t gz = readInt16(QMI8658C_GZ_L);
    
    // Read temperature
    int16_t temp = readInt16(QMI8658C_TEMP_L);
    
    // Convert to physical units
    data.accelX = ax * accelScale;
    data.accelY = ay * accelScale;
    data.accelZ = az * accelScale;
    
    data.gyroX = gx * gyroScale;
    data.gyroY = gy * gyroScale;
    data.gyroZ = gz * gyroScale;
    
    data.temperature = temp / 256.0;  // Temperature conversion
}

IMUData QMI8658C::getData() {
    return data;
}

uint8_t QMI8658C::readRegister(uint8_t reg) {
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(_addr, (uint8_t)1);
    return _wire->read();
}

void QMI8658C::writeRegister(uint8_t reg, uint8_t value) {
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(value);
    _wire->endTransmission();
}

int16_t QMI8658C::readInt16(uint8_t regLow) {
    uint8_t low = readRegister(regLow);
    uint8_t high = readRegister(regLow + 1);
    return (int16_t)((high << 8) | low);
}



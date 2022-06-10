#ifndef BMP085_H
#define BMP085_H

#define BMP085_ADDR 0x77
void initBmp085();
float readTemperature();
long readPressure();

#endif

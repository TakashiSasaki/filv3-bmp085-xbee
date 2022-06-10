#ifndef XBEES1_H
#define XBEES1_H
extern "C" XBee xbee;

void sendData(const uint8_t* dh, const uint8_t* sl, const char* tag, float f);



#endif

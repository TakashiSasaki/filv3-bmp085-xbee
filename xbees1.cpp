#include <stdio.h>
#include <XBee.h>
#include "xbees1.h"

XBee xbee;

void sendData(const uint8_t* dh, const uint8_t* dl, const char* tag, float f){
    uint8_t buffer[100];
    const int len = sprintf(buffer, "%s %f", tag, f);
    uint8_t dst_addr[8];
    memcpy(dst_addr, dh, 4);
    memcpy(dst_addr+4, dl, 4);
    Tx64Request tx64Request(dst_addr, buffer, len);
    xbee.send(tx64Request);
}

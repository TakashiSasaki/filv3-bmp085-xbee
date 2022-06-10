#ifndef XBEES1_H
#define XBEES1_H

extern "C" XBee xbee;
extern "C" bool PC1;
extern "C" uint8_t PC1SH[4];
extern "C" uint8_t PC1SL[4];
extern "C" bool PC2;
extern "C" uint8_t PC2SH[4];
extern "C" uint8_t PC2SL[4];


void sendData(const uint8_t* dh, const uint8_t* sl, const char* tag, float f);
void onAtCommandResponseND(const AtCommandResponse& atCommandResponse);
void onAtCommandResponse();


#endif

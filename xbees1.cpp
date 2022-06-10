#include <stdio.h>
#include <XBee.h>
#include "xbees1.h"

XBee xbee;
bool PC1 = false;
uint8_t PC1SH[4];
uint8_t PC1SL[4];
bool PC2 = false;
uint8_t PC2SH[4];
uint8_t PC2SL[4];

void sendData(const uint8_t* dh, const uint8_t* dl, const char* tag, float f) {
  uint8_t buffer[100];
  const int len = sprintf(buffer, "%s %f", tag, f);
  //uint8_t dst_addr[8];
  //memcpy(dst_addr, dh, 4);
  //memcpy(dst_addr+4, dl, 4);
  XBeeAddress64 xbeeAddress64 = XBeeAddress64(
                                  (((unsigned long)dh[0]) << 24) | (((unsigned long)dh[1]) << 16) | (((unsigned long)dh[2]) << 8) | (unsigned long)dh[3],
                                  (((unsigned long)dl[0]) << 24) | (((unsigned long)dl[1]) << 16) | (((unsigned long)dl[2]) << 8) | (unsigned long)dl[3]);
  Tx64Request tx64Request(xbeeAddress64, buffer, len);
  xbee.send(tx64Request);
}

void onAtCommandResponse() {
  AtCommandResponse atCommandResponse = AtCommandResponse();
  xbee.getResponse().getAtCommandResponse(atCommandResponse);
  const char c0 = atCommandResponse.getCommand()[0];
  const char c1 = atCommandResponse.getCommand()[1];
  Serial.print(c0);
  Serial.print(c1);
  Serial.print(' ');
  if (c0 == 'N' && c1 == 'I') {
    for (int i = 0; i < atCommandResponse.getValueLength(); ++i) {
      const char x = atCommandResponse.getValue()[i];
      Serial.print((char)(atCommandResponse.getValue()[i]));
    }//for
  } else {
    for (int i = 0; i < atCommandResponse.getValueLength(); ++i) {
      const unsigned char x = atCommandResponse.getValue()[i];
      if (x < 0x10) Serial.print('0');
      Serial.print((unsigned char)(atCommandResponse.getValue()[i]), HEX);
    }//for
    if (c0 == 'N' && c1 == 'D') {
      onAtCommandResponseND(atCommandResponse);
    }
  }//if

}

void onAtCommandResponseND(const AtCommandResponse& atCommandResponse) {
  if (atCommandResponse.getValueLength() >= 11) {
    uint8_t my[2];
    my[0] = atCommandResponse.getValue()[0];
    my[1] = atCommandResponse.getValue()[1];
    uint8_t sh[4];
    sh[0] = atCommandResponse.getValue()[2];
    sh[1] = atCommandResponse.getValue()[3];
    sh[2] = atCommandResponse.getValue()[4];
    sh[3] = atCommandResponse.getValue()[5];
    uint8_t sl[4];
    sl[0] = atCommandResponse.getValue()[6];
    sl[1] = atCommandResponse.getValue()[7];
    sl[2] = atCommandResponse.getValue()[8];
    sl[3] = atCommandResponse.getValue()[9];
    const uint8_t db = atCommandResponse.getValue()[10];

    char ni[21];
    int i = 0;
    const int ni_len = atCommandResponse.getValueLength() - 11;
    for (i = 0; i < ni_len; ++i) {
      ni[i] = atCommandResponse.getValue()[i + 10];
    }//for
    ni[i + 1] = '\0';

    if (strcmp("PC1", ni) == 0) {
      PC1 = true;
      memcpy(PC1SH, sh, 4);
      memcpy(PC1SL, sl, 4);
    } else if (strcmp("PC2", ni) == 0) {
      PC2 = true;
      memcpy(PC2SH, sh, 4);
      memcpy(PC2SL, sl, 4);
    }//if

    Serial.print(" (");
    Serial.print(ni);
    Serial.print(")");

  }//if
}//onAtCommandResponseND

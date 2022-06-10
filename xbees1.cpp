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
uint8_t NI[21];

void SendData(const uint8_t* dh, const uint8_t* dl, const char* tag, float f) {
  uint8_t buffer[100];
  uint8_t buffer2[100];
  dtostrf(f, 6, 2, buffer2);
  const int len = sprintf(buffer, "%s %s", tag, buffer2);
  //uint8_t dst_addr[8];
  //memcpy(dst_addr, dh, 4);
  //memcpy(dst_addr+4, dl, 4);
  const uint32_t addr_high = (((uint32_t)dh[0]) << 24) | (((uint32_t)dh[1]) << 16) | (((uint32_t)dh[2]) << 8) | (uint32_t)dh[3];
  Serial.print("ADDR_HIGH :");
  Serial.print(addr_high, HEX);
  Serial.print('\n');
  const uint32_t addr_low = (((uint32_t)dl[0]) << 24) | (((uint32_t) dl[1]) << 16) | (((uint32_t)dl[2]) << 8) | (uint32_t)dl[3];
  Serial.print("ADDR_LOW  :");
  Serial.print(addr_low, HEX);
  Serial.print('\n');
  const uint16_t addr_16 = (((uint16_t)dl[2]) << 8) | (uint16_t)dl[3];

  XBeeAddress64 xbeeAddress64 = XBeeAddress64(addr_high, addr_low);
  Tx64Request tx64Request(xbeeAddress64, buffer, strlen(buffer));
  //Tx16Request tx16Request(addr_16, buffer, strlen(buffer));
  //xbee.send(tx64Request);
  //xbee.send(tx16Request);
  ZBTxRequest zbTxRequest = ZBTxRequest(xbeeAddress64, buffer, strlen(buffer));
  xbee.send(zbTxRequest);
  delay(1000);
  Serial.print("SENT ");
  Serial.print((char*)buffer);
  Serial.print('\n');
}//sendData

void SendDigiMeshData(const uint8_t* dh, const uint8_t* dl, const uint8_t * buffer, int len) {
  uint8_t checksum = 0;
  Serial1.write('\x7E');
  Serial1.write(0);
  Serial1.write(len + 14);
  Serial1.write(0x10); checksum += 0x10;
  Serial1.write(0x01); checksum += 0x01;
  Serial1.write(dh[0]); checksum += dh[0];
  Serial1.write(dh[1]); checksum += dh[1];
  Serial1.write(dh[2]); checksum += dh[2];
  Serial1.write(dh[3]); checksum += dh[3];
  Serial1.write(dl[0]); checksum += dl[0];
  Serial1.write(dl[1]); checksum += dl[1];
  Serial1.write(dl[2]); checksum += dl[2];
  Serial1.write(dl[3]); checksum += dl[3];
  Serial1.write(0xFF); checksum += 0xFF;
  Serial1.write(0xFE); checksum += 0xFE;
  Serial1.write(0x00); //Broadcast Radius
  Serial1.write(0x00); //Options
  Serial1.write((char*)buffer);
  for (int i = 0; i < len; ++i) {
    checksum += buffer[i]; //RF Data
  }
  Serial1.write(0xFF - checksum);
  //Serial.write("SendDigiMeshData\n");
}//SendDigiMeshData

void SendDigiMeshLong(const uint8_t* dh, const uint8_t* dl, const char* tag, long l) {
  uint8_t buffer[100];
  const int len = sprintf(buffer, "%s %s %ld", NI, tag, l);
  SendDigiMeshData(dh, dl, buffer, len);
}

void SendDigiMeshFloat(const uint8_t* dh, const uint8_t* dl, const char* tag, float f) {
  uint8_t buffer[100];
  uint8_t buffer2[100];
  dtostrf(f, 6, 2, buffer2);
  const int len = sprintf(buffer, "%s %s %s", NI, tag, buffer2);
  SendDigiMeshData(dh, dl, buffer, len);
}

void onTxStatusResponse() {
  TxStatusResponse txStatusResponse = TxStatusResponse();
  xbee.getResponse().getTxStatusResponse(txStatusResponse);
  Serial.print("TX ");
  Serial.print(txStatusResponse.getStatus() == SUCCESS ? "OK" : "NG");
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
    int i = 0;
    for (; i < atCommandResponse.getValueLength(); ++i) {
      const char x = atCommandResponse.getValue()[i];
      NI[i] = x;
      Serial.print((char)(atCommandResponse.getValue()[i]));
    }//for
    NI[i] = '\0';
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
    ni[i] = '\0';

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

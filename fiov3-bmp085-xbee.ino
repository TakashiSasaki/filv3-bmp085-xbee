#include <SoftwareSerial.h>
#include <Wire.h>
#include <Printers.h>
#include <XBee.h>
#include <avr/sleep.h>
#include "bmp085.h"
#include "morse.h"

//Connecting pin 8 and 9 to TX and RX of USB serial device respectively.
XBee xbee;

void sendATSH() {
  const uint8_t ATSH[] = {'S', 'H'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATSH);
  xbee.send(atCommandRequest);
}

void sendATSL() {
  const uint8_t ATSL[] = {'S', 'L'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATSL);
  xbee.send(atCommandRequest);
}

void sendATNI() {
  const uint8_t ATNI[] = {'N', 'I'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATNI);
  xbee.send(atCommandRequest);
}

void sendATNDPC1() {
  const uint8_t ATND[] = {'N', 'D'};
  const uint8_t PC1[] = {'P', 'C', '1'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATND, PC1, 3);
  //AtCommandRequest atCommandRequest = AtCommandRequest(ATND);
  xbee.send(atCommandRequest);
}

bool getResponse() {
  if (xbee.readPacket(5000)) {
    //Serial.print(" Read \n");
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
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
      }//if

      if (c0 == 'N' && c1 == 'D') {
        if (atCommandResponse.getValueLength() >= 11) {
          uint8_t my[2];
          my[0] = atCommandResponse.getValue()[0];
          my[1] = atCommandResponse.getValue()[1];
          uint8_t sh[4];
          sh[0] = atCommandResponse.getValue()[2];
          sh[0] = atCommandResponse.getValue()[3];
          sh[0] = atCommandResponse.getValue()[4];
          sh[0] = atCommandResponse.getValue()[5];
          uint8_t sl[4];
          sh[0] = atCommandResponse.getValue()[6];
          sh[0] = atCommandResponse.getValue()[7];
          sh[0] = atCommandResponse.getValue()[8];
          sh[0] = atCommandResponse.getValue()[9];
          const uint8_t db = atCommandResponse.getValue()[10];

          char ni[21];
          int i = 0;
          const int ni_len = atCommandResponse.getValueLength() - 11;
          for (i = 0; i < ni_len; ++i) {
            ni[i] = atCommandResponse.getValue()[i + 10];
          }
          ni[i + 1] = '\0';
          Serial.print(" (");
          Serial.print(ni);
          Serial.print(")");
        }//if
      }
    }//if
    Serial.print('\n');
  }//if
  return xbee.getResponse().isAvailable();
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  xbee = XBee();
  xbee.setSerial(Serial1);

  // put your setup code here, to run once:
  //Serial.begin(9600);
  Wire.begin();
  initBmp085();

  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 3);
}//setup

void loop() {
  set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
  // put your main code here, to run repeatedly:

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_L);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 7);

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_T);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 7);

  const float temperature = readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print('\n');

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_T);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_E);  
  delay(MORSE_SHORT_MSEC * 7);

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_P);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 7);

  const long pressure = readPressure();
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print('\n');

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_P);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_E);  
  delay(MORSE_SHORT_MSEC * 7);

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_X);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_S);  
  delay(MORSE_SHORT_MSEC * 7);

  sendATSH();
  getResponse();
  sendATSL();
  getResponse();
  sendATNI();
  getResponse();
  sendATNDPC1();
  getResponse();
  while (getResponse());

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_X);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_E);  
  delay(MORSE_SHORT_MSEC * 7);

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_L);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_E);  
  delay(MORSE_SHORT_MSEC * 7);
}

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Printers.h>
#include <XBee.h>
#include <avr/sleep.h>
#include "bmp085.h"
#include "morse.h"
#include "xbees1.h"

//Connecting pin 8 and 9 to TX and RX of USB serial device respectively.
//XBee xbee;


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
      onAtCommandResponse();
    }//if
    Serial.print('\n');
  }//if
  return xbee.getResponse().isAvailable();
}


void printHex(const uint8_t *p, int len) {
  for (int i = 0; len > 0; --len, ++i) {
    if (p[i] < 0x10) {
      Serial.print('0');
    }
    Serial.print(p[i], HEX);
  }
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

  if (PC1) {
    Serial.print("PC1 ");
    printHex(PC1SH, 4);
    printHex(PC1SL, 4);
    Serial.print('\n');
    sendData(PC1SH, PC1SL, "T", temperature);
    delay(MORSE_SHORT_MSEC * 7);
    morse_letter(MORSE_1);
    delay(MORSE_SHORT_MSEC * 7);
  }

  if (PC2) {
    Serial.print("PC2 ");
    printHex(PC2SH, 4);
    printHex(PC2SL, 4);
    Serial.print('\n');
    delay(MORSE_SHORT_MSEC * 7);
    morse_letter(MORSE_2);
    delay(MORSE_SHORT_MSEC * 7);
  }

  delay(MORSE_SHORT_MSEC * 7);
  morse_letter(MORSE_L);
  delay(MORSE_SHORT_MSEC * 3);
  morse_letter(MORSE_E);
  delay(MORSE_SHORT_MSEC * 7);
}

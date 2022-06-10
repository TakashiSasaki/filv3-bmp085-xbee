#include <SoftwareSerial.h>
#include <Printers.h>
#include <XBee.h>
#include <avr/sleep.h>
#include <Wire.h>
const int RXLED=17;
#define BMP085_ADDR 0x77

const int MORSE_SHORT_MSEC = 120;
const int MORSE_ADJ_MSEC = 30;
const char* const MORSE_A = ".-";
const char* const MORSE_B = "-...";
const char* const MORSE_C = "-.-.";
const char* const MORSE_D = "-..";
const char* const MORSE_E = ".";
const char* const MORSE_F = "..-.";
const char* const MORSE_G = "--.";
const char* const MORSE_H = "....";
const char* const MORSE_I = "..";
const char* const MORSE_J = ".---";
const char* const MORSE_K = "-.-";
const char* const MORSE_L = ".-..";
const char* const MORSE_M = "--";
const char* const MORSE_N = "-.";
const char* const MORSE_O = "---";
const char* const MORSE_P = ".--.";
const char* const MORSE_Q = "--.-";
const char* const MORSE_R = ".-.";
const char* const MORSE_S = "...";
const char* const MORSE_T = "-";
const char* const MORSE_U = "..-";
const char* const MORSE_V = "...-";
const char* const MORSE_W = ".--";
const char* const MORSE_X = "-..-";
const char* const MORSE_Y = "-.--";
const char* const MORSE_Z = "--..";

int ac1, ac2, ac3;
unsigned int ac4, ac5, ac6;
int b1, b2, mb, mc, md;
long b5;

//Connecting pin 8 and 9 to TX and RX of USB serial device respectively.
XBee xbee;

void atSH(){
  const uint8_t ATSH[] = {'S','H'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATSH);
  AtCommandResponse atCommandResponse = AtCommandResponse();
  xbee.send(atCommandRequest);
  //Serial.print(" Sent \n");
  if(xbee.readPacket(5000)){
    //Serial.print(" Read \n");
    if(xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atCommandResponse);
      Serial.print((char)atCommandResponse.getCommand()[0]);
      Serial.print((char)atCommandResponse.getCommand()[1]); 
      Serial.print(' ');
      for(int i=0; i< atCommandResponse.getValueLength(); ++i){
        const unsigned char x = atCommandResponse.getValue()[i];
        if(x < 0x10) Serial.print('0');
        Serial.print((unsigned char)(atCommandResponse.getValue()[i]), HEX);
      }//for
    }
    Serial.print('\n');
  }
}

void atSL(){
  const uint8_t ATSL[] = {'S','L'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATSL);
  AtCommandResponse atCommandResponse = AtCommandResponse();
  xbee.send(atCommandRequest);
  //Serial.print(" Sent \n");
  if(xbee.readPacket(5000)){
    //Serial.print(" Read \n");
    if(xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atCommandResponse);
      Serial.print((char)atCommandResponse.getCommand()[0]);
      Serial.print((char)atCommandResponse.getCommand()[1]); 
      Serial.print(' ');
      for(int i=0; i< atCommandResponse.getValueLength(); ++i){
        const unsigned char x = atCommandResponse.getValue()[i];
        if(x < 0x10) Serial.print('0');
        Serial.print((unsigned char)(atCommandResponse.getValue()[i]), HEX);
      }//for
    }
    Serial.print('\n');
  }
}

void atNI(){
  const uint8_t ATNI[] = {'N','I'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATNI);
  AtCommandResponse atCommandResponse = AtCommandResponse();
  xbee.send(atCommandRequest);
  //Serial.print(" Sent \n");
  if(xbee.readPacket(5000)){
    //Serial.print(" Read \n");
    if(xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atCommandResponse);
      Serial.print((char)atCommandResponse.getCommand()[0]);
      Serial.print((char)atCommandResponse.getCommand()[1]); 
      Serial.print(' ');
      for(int i=0; i< atCommandResponse.getValueLength(); ++i){
        const char x = atCommandResponse.getValue()[i];
        Serial.print((char)(atCommandResponse.getValue()[i]));
      }//for
    }
    Serial.print('\n');
  }
}


void atNDPC1(){
  const uint8_t ATND[] = {'N','D'};
  const uint8_t PC1[] = {'P', 'C', '1', '\0'};
  AtCommandRequest atCommandRequest = AtCommandRequest(ATND, PC1, 3);
  AtCommandResponse atCommandResponse = AtCommandResponse();
  xbee.send(atCommandRequest);
  //Serial.print(" Sent \n");
  if(xbee.readPacket(10000)){
    //Serial.print(" Read \n");
    if(xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atCommandResponse);
      Serial.print((char)atCommandResponse.getCommand()[0]);
      Serial.print((char)atCommandResponse.getCommand()[1]); 
      Serial.print(' ');
      for(int i=0; i< atCommandResponse.getValueLength(); ++i){
        const unsigned char x = atCommandResponse.getValue()[i];
        if(x < 0x10) Serial.print('0');
        Serial.print((unsigned char)(atCommandResponse.getValue()[i]), HEX);
      }//for
    }
    Serial.print('\n');
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

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xAA);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  //unsigned char msb, lsb;
  const unsigned char ac1_msb = Wire.read();
  const unsigned char ac1_lsb = Wire.read();
  ac1 = (int)ac1_msb << 8 | ac1_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xAC);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char ac2_msb = Wire.read();
  const unsigned char ac2_lsb = Wire.read();
  ac2 = (int) ac2_msb << 8 | ac2_lsb;
  
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xAE);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char ac3_msb = Wire.read();
  const unsigned char ac3_lsb = Wire.read();
  ac3 = (int) ac3_msb << 8 | ac3_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xB0);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char ac4_msb = Wire.read();
  const unsigned char ac4_lsb = Wire.read();
  ac4 = (int) ac4_msb << 8 | ac4_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xB2);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char ac5_msb = Wire.read();
  const unsigned char ac5_lsb = Wire.read();
  ac5 = (int) ac5_msb << 8 | ac5_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xB4);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char ac6_msb = Wire.read();
  const unsigned char ac6_lsb = Wire.read();
  ac6 = (int) ac6_msb << 8 | ac6_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xB6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char b1_msb = Wire.read();
  const unsigned char b1_lsb = Wire.read();
  b1 = (int) b1_msb << 8 | b1_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xB8);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char b2_msb = Wire.read();
  const unsigned char b2_lsb = Wire.read();
  b2 = (int) b2_msb << 8 | b2_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xBA);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char mb_msb = Wire.read();
  const unsigned char mb_lsb = Wire.read();
  mb = (int) mb_msb << 8 | mb_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xBC);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char mc_msb = Wire.read();
  const unsigned char mc_lsb = Wire.read();
  mc = (int) mc_msb << 8 | mc_lsb;

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xBE);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char md_msb = Wire.read();
  const unsigned char md_lsb = Wire.read();
  md = (int) md_msb << 8 | md_lsb;
}//setup

void morse_short(){
  TXLED0;
  delay(MORSE_SHORT_MSEC + MORSE_ADJ_MSEC);
  TXLED1;
  delay(MORSE_SHORT_MSEC - MORSE_ADJ_MSEC);
}

void morse_long(){
  TXLED0;
  delay(MORSE_SHORT_MSEC * 3 + MORSE_ADJ_MSEC);
  TXLED1;
  delay(MORSE_SHORT_MSEC - MORSE_ADJ_MSEC);
}

void morse_letter(const char* p){
  while(*p != 0){
    switch(*p++) {
      case '.':
        morse_short();
        break;
      case '-':
      case '_':
        morse_long();
        break;
    }//switch
  }//while
}//morse_letter


float temperature;
long pressure;

void loop() {
  TXLED1;
  set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
  // put your main code here, to run repeatedly:
  morse_letter(MORSE_B);
  delay(MORSE_SHORT_MSEC * 2);

  readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print('\n');

  readPressure();
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print('\n');
  
  atSH();
  atSL();
  atNI();
  atNDPC1();
  delay(10000);
}

void readTemperature(){
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  delay(10);
  
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 2);
  while(Wire.available() < 2);
  const unsigned char msb = Wire.read();
  const unsigned char lsb = Wire.read();
  const unsigned int raw_temp = msb << 8 | lsb;
  const long x1 = (((long)raw_temp - (long)ac6)*(long)ac5) >> 15;
  const long x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;
  temperature = ((b5+8) >> 4)/10.0;
}

#define OVERSAMPLING 0 
void readPressure(){
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF4);
  Wire.write(0x34 + (OVERSAMPLING << 6)); //0 is oversampling rate
  Wire.endTransmission();
  delay(2 + (3<<OVERSAMPLING));
  
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 1);
  while(!Wire.available());
  const unsigned char msb = Wire.read();
  
  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF7);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 1);
  while(!Wire.available());
  const unsigned char lsb = Wire.read();

  Wire.beginTransmission(BMP085_ADDR);
  Wire.write(0xF8);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDR, 1);
  while(!Wire.available());
  const unsigned char xlsb = Wire.read();

  const unsigned long raw_pressure = 
    (
      ((unsigned long) msb << 16) 
      | ((unsigned long) lsb << 8) 
      | (unsigned long) xlsb
    ) 
    >> (8-OVERSAMPLING);

  const long b6 = b5 - 4000;
  const long x1 = (b2 * (b6 * b6) >> 12) >> 11;
  const long x2 = (ac2 * b6) >> 11;
  const long x3 = x1 + x2;
  const long b3 = (( ( ((long)ac1)*4 + x3 ) << OVERSAMPLING) + 2) >> 2;

  const long y1 = (ac3 * b6) >> 13;
  const long y2 = (b1 * ( (b6*b6)>>12 )) >> 16;
  const long y3 = ((y1 + y2) + 2) >> 2;
  const unsigned long b4 = (ac4 * (unsigned long)(y3 + 32768)) >> 15;
  const unsigned long b7 = ((unsigned long)(raw_pressure - b3) * (50000 >> OVERSAMPLING));

  const long p = (b7 < 0x80000000) ? ((b7<<1)/b4) : ((b7/b4)<<1);

  const long z1 = (p >> 8) * (p >> 8);
  const long z2 = (z1 * 3038) >> 16;
  const long z3 = (-7357 * p) >> 16;
  pressure = p + ((z2 + z3 + 3791) >> 4);
}//readPressure

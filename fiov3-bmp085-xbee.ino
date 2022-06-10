#include <SoftwareSerial.h>
#include <Printers.h>
#include <XBee.h>
#include <avr/sleep.h>
#include "bmp085.h"
#include "morse.h"

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

}//setup


//float temperature;
//long pressure;

void loop() {
  TXLED1;
  set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
  // put your main code here, to run repeatedly:
  morse_letter(MORSE_B);
  delay(MORSE_SHORT_MSEC * 2);

  const float temperature = readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print('\n');

  const long pressure = readPressure();
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print('\n');
  
  atSH();
  atSL();
  atNI();
  atNDPC1();
  delay(10000);
}

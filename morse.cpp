#include <Arduino.h>
#include "morse.h"

void morse_short(){
  RXLED1;
  RXLED0;
  delay(MORSE_SHORT_MSEC + MORSE_ADJ_MSEC);
  RXLED1;
}

void morse_long(){
  RXLED1;
  RXLED0;
  delay(MORSE_SHORT_MSEC * 3 + MORSE_ADJ_MSEC);
  RXLED1;
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
    delay(MORSE_SHORT_MSEC - MORSE_ADJ_MSEC);
  }//while
}//morse_letter

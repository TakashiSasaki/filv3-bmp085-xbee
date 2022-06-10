#include <Arduino.h>
#include "morse.h"

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

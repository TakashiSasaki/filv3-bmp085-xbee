#ifndef MORSE_H
#define MORSE_H

#define RXLED 17

#define MORSE_SHORT_MSEC 120
#define MORSE_ADJ_MSEC 30

#define MORSE_A  ".-"
#define MORSE_B  "-..."
#define MORSE_C  "-.-."
#define MORSE_D  "-.."
#define MORSE_E  "."
#define MORSE_F  "..-."
#define MORSE_G  "--."
#define MORSE_H  "...."
#define MORSE_I  ".."
#define MORSE_J  ".---"
#define MORSE_K  "-.-"
#define MORSE_L  ".-.."
#define MORSE_M  "--"
#define MORSE_N  "-."
#define MORSE_O  "---"
#define MORSE_P  ".--."
#define MORSE_Q  "--.-"
#define MORSE_R  ".-."
#define MORSE_S  "..."
#define MORSE_T  "-"
#define MORSE_U  "..-"
#define MORSE_V  "...-"
#define MORSE_W  ".--"
#define MORSE_X  "-..-"
#define MORSE_Y  "-.--"
#define MORSE_Z  "--.."

void morse_letter(const char* p);

#endif

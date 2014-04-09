// using 2 8-bit shift registers in series to power 2 7-segment displays

#include <Tone.h>
#include <Keypad.h>

void keypadEvent(KeypadEvent key);
void displayNumber(int number);
void displayLeftDigit(int digit);
void displayRightDigit(int digit);
void updateDisplayValue(int num);

Tone freq1;
Tone freq2;

const int SHIFT_DATA_PIN = 11;
const int SHIFT_CLOCK_PIN = 10;
const int SHIFT_LATCH_PIN = 9;

const int DTMF_freq1[] = { 1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477, 1209 /* * */, 1477 /* # */ };
const int DTMF_freq2[] = {  941,  697,  697,  697,  770,  770,  770,  852,  852,  852, 941 /* * */, 941 /* # */ };

const byte KEYPAD_ROWS = 4; //four rows
const byte KEYPAD_COLS = 3; //three columns
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[KEYPAD_ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[KEYPAD_COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

/*
  keypad 3 - arduino digital pin 2
  keypad 1 - arduino digital pin 3
  keypad 5 - arduino digital pin 4

  keypad 2 - arduino digital pin 5
  keypad 7 - arduino digital pin 6
  keypad 6 - arduino digital pin 7
  keypad 4 - arduino digital pin 8
*/

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS );

int displayValue;

const int DISPLAY_DOTS = -1;
const int DISPLAY_SEG_DOT = -1;
const int DISPLAY_DASHES = -2;
const int DISPLAY_SEG_DASH = -2;
const int DISPLAY_CLEAR = -3;
const int DISPLAY_SEG_CLEAR = -3;

void setup()
{
  freq1.begin(12);
  freq2.begin(13);
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);

  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);

  displayValue = DISPLAY_DOTS;
  displayNumber(displayValue);
}

void loop()
{
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }

}


void updateDisplayValue(int num)
{
  if (num < 0) {
    if (num == DISPLAY_DASHES && displayValue == DISPLAY_DASHES) {
      displayValue = DISPLAY_CLEAR;
    } else {
      displayValue = num;
    }
  } else {
    if (displayValue < 0) {
      displayValue = num;
    } else {
      displayValue = (displayValue % 10) * 10 + num;
    }
  }
  displayNumber(displayValue);
}

void keypadEvent(KeypadEvent key)
{
  int num = 0;
  if (key == '*') {
    num = DISPLAY_DOTS;
  } else if (key == '#') {
    num = DISPLAY_DASHES;
  } else {
    num = key - '0';
  }

  switch (keypad.getState()){
  case PRESSED:
    freq1.play(DTMF_freq1[num]);
    freq2.play(DTMF_freq2[num]);

    updateDisplayValue(num);
    break;

  case RELEASED:
    freq1.stop();
    freq2.stop();
    break;

  case HOLD:
    break;
  }

}

enum {
  SEG_TOP,
  SEG_UR,
  SEG_DOT,
  SEG_LR,
  SEG_BOT,
  SEG_LL,
  SEG_MID,
  SEG_UL
};

char byteForNumber(int num)
{
  char byte = 0;
  switch (num) {
  case 0:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_LL;
    byte |= 1 << SEG_UL;
    break;

  case 1:
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    break;

  case 2:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_LL;
    byte |= 1 << SEG_MID;
    break;

  case 3:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_MID;
    break;

  case 4:
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_UL;
    byte |= 1 << SEG_MID;
    break;

  case 5:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_UL;
    byte |= 1 << SEG_MID;
    break;

  case 6:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_LL;
    byte |= 1 << SEG_UL;
    byte |= 1 << SEG_MID;
    break;

  case 7:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    break;

  case 8:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_LL;
    byte |= 1 << SEG_UL;
    byte |= 1 << SEG_MID;
    break;

  case 9:
    byte |= 1 << SEG_TOP;
    byte |= 1 << SEG_UR;
    byte |= 1 << SEG_LR;
    byte |= 1 << SEG_BOT;
    byte |= 1 << SEG_UL;
    byte |= 1 << SEG_MID;
    break;

  case DISPLAY_SEG_DOT:
    byte |= 1 << SEG_DOT;
    break;

  case DISPLAY_SEG_DASH:
    byte |= 1 << SEG_MID;
    break;

  case DISPLAY_SEG_CLEAR:
    byte = 0;
    break;

  default:
    break;
  }

  return byte;
}

void displayNumber(int number)
{
  int leftDigit = 0; 
  int rightDigit = 0;

  if (number == DISPLAY_DOTS) {
    leftDigit = DISPLAY_SEG_DOT;
    rightDigit = DISPLAY_SEG_DOT;
  } else if (number == DISPLAY_DASHES) {
    leftDigit = DISPLAY_SEG_DASH;
    rightDigit = DISPLAY_SEG_DASH;
  } else if (number == DISPLAY_CLEAR) {
    leftDigit = DISPLAY_SEG_CLEAR;
    rightDigit = DISPLAY_SEG_CLEAR;
  } else {
    leftDigit = number / 10;
    rightDigit = number % 10;
    if (leftDigit == 0) {
      leftDigit = DISPLAY_SEG_CLEAR;
    }
 }
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  displayLeftDigit(leftDigit);
  displayRightDigit(rightDigit);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void displayLeftDigit(int digit)
{
  char byte = byteForNumber(digit);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, byte);
}

void displayRightDigit(int digit)
{
  char byte = byteForNumber(digit);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, byte);
}
//void lightUpDigit(int DisplayNumber);

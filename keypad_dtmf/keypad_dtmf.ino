
#include <Tone.h>
#include <Keypad.h>

void keypadEvent(KeypadEvent key);

Tone freq1;
Tone freq2;

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

void setup()
{
  freq1.begin(12);
  freq2.begin(13);
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
}

void loop()
{
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }

}

void keypadEvent(KeypadEvent key)
{
  int num = 0;
  if (key == '*') {
    num = 10;
  } else if (key == '#') {
    num = 11;
  } else {
    num = key - '0';
  }

  switch (keypad.getState()){
  case PRESSED:
    freq1.play(DTMF_freq1[num]);
    freq2.play(DTMF_freq2[num]);
    break;

  case RELEASED:
    freq1.stop();
    freq2.stop();
    break;

  case HOLD:
    break;
  }

}
#include <Tone.h>
#include <Keypad.h>

void keypadEvent(KeypadEvent key);
void displayNumber(int number);
void displayNumberWithDot(int number, int withDot);
void displayLeftDigit(int digit);
void displayRightDigitWithDot(int digit, int withDot);
void updateDisplayValue(int num);
void handleStopIdKeypress(KeypadEvent key);
int keyToNumber(KeypadEvent key);
void playDTMF(int digit);
void stopDTMF();
void playErrorTones();
void readSerialForMinutes();
void displayRoutesAndMinutes();
void resetCyclingRoutesMinutes();

Tone freq1;
Tone freq2;

char stopIdBuffer[12];
char *stopIdBufferIndex = stopIdBuffer;

const int SHIFT_DATA_PIN = 11;
const int SHIFT_CLOCK_PIN = 10;
const int SHIFT_LATCH_PIN = 9;

const int GREEN_LED_PIN = A4;
const int RED_LED_PIN = A5;

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

const int BUFFER_SIZE = 256;
char serialBuffer[BUFFER_SIZE];
char *serialBufferIndex = serialBuffer;

int minutes[10];
int routes[10];
int arridx = 0;
int arrsize = 0;

enum {
  G_STATE_CYCLE_ROUTES_MINUTES,
  G_STATE_KEYPAD_INPUT
};

int globalState = G_STATE_KEYPAD_INPUT;

void setup()
{
  freq1.begin(12);
  freq2.begin(13);
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);

  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  displayValue = DISPLAY_DOTS;
  displayNumber(displayValue);
}

void loop()
{
  (void)keypad.getKey();

  readSerialForMinutes();

  displayRoutesAndMinutes();

}

int curarridx = 0;
unsigned long lastTransitionTime = 0;
unsigned long transitionInterval = 2000;

enum {
  DISPLAY_STATE_ROUTE,
  DISPLAY_STATE_MINUTES,
};

int curDisplayState = DISPLAY_STATE_ROUTE;
int prevDisplayState = -1;

void setGreenLED(int value)
{
  digitalWrite(GREEN_LED_PIN, value);
}

void setRedLED(int value)
{
  digitalWrite(RED_LED_PIN, value);
}

void displayRoutesAndMinutes()
{
  if (arrsize && globalState == G_STATE_CYCLE_ROUTES_MINUTES) {

    unsigned long now = millis();

    if (now - lastTransitionTime > transitionInterval) {

      if (curDisplayState == DISPLAY_STATE_ROUTE) {
	//Serial.println(98);
	displayNumber(DISPLAY_CLEAR);
	delay(50);
	displayNumberWithDot(routes[curarridx], 1);

	if (minutes[curarridx] > 5) {
	  setGreenLED(HIGH);
	  setRedLED(LOW);
	} else {
	  setGreenLED(LOW);
	  setRedLED(HIGH);
	}

	prevDisplayState = DISPLAY_STATE_ROUTE;
	curDisplayState = DISPLAY_STATE_MINUTES;
      } else if (curDisplayState == DISPLAY_STATE_MINUTES && prevDisplayState != DISPLAY_STATE_MINUTES) {
	//Serial.println(97);
	displayNumber(DISPLAY_CLEAR);
	delay(50);
	displayNumber(minutes[curarridx]);

	curarridx = (curarridx + 1) % arrsize;

	prevDisplayState = DISPLAY_STATE_MINUTES;
	if (arrsize > 1) {
	  // don't cycle routes/minutes if only showing one route
	  curDisplayState = DISPLAY_STATE_ROUTE;
	}
      }
      lastTransitionTime = now;
    }

  }
}

//////////////
/// SERIAL ///
//////////////

int atoi(char buffer[]) {
  int idx = 0;
  int ret = 0;
  while (buffer[idx] != 0) {
    ret = ret * 10;
    char c = buffer[idx];
    ret = ret + (c - '0');
    idx++;
  }
  return ret;
}

void readSerialForMinutes() {
  while (Serial.available() > 0) { // check if there is data waiting
    int byte = Serial.read(); // read one byte
    if (byte != 10) { // if byte is not newline

      if (byte == '.') {
	*serialBufferIndex = '\0';
	int num = atoi(serialBuffer);
	routes[arridx] = num;
	serialBufferIndex = serialBuffer;

      } else if (byte == '|') {
	*serialBufferIndex = '\0';
	int num = atoi(serialBuffer);
	minutes[arridx] = num;
	serialBufferIndex = serialBuffer;
	arridx++;

      } else {
	// extend the buffer
	*serialBufferIndex = char(byte);
	serialBufferIndex++;
      }

    } else {
      arrsize = arridx;
      arridx = 0;
      serialBufferIndex = serialBuffer;

      // reset cycling routes/minutes
      resetCyclingRoutesMinutes();
    }

  }
}

void resetCyclingRoutesMinutes()
{
  globalState = G_STATE_CYCLE_ROUTES_MINUTES;
  curDisplayState = DISPLAY_STATE_ROUTE;
  curarridx = 0;
  lastTransitionTime = 0;
}

////////////////
//// KEYPAD ////
////////////////

int keyToNumber(KeypadEvent key)
{
  int num = 0;
  if (key == '*') {
    num = DISPLAY_DOTS;
  } else if (key == '#') {
    num = DISPLAY_DASHES;
  } else {
    num = key - '0';
  }

  return num;
}

void keypadEvent(KeypadEvent key)
{

  int num = keyToNumber(key);

  switch (keypad.getState()){
  case PRESSED:
    if (globalState == G_STATE_CYCLE_ROUTES_MINUTES) {
      displayNumber(DISPLAY_CLEAR);
    }
    playDTMF(num);
    updateDisplayValue(num);
    handleStopIdKeypress(key);
    break;

  case RELEASED:
    stopDTMF();
    break;

  case HOLD:
    break;
  }

}

///////////////
/// Stop ID ///
///////////////

void handleStopIdKeypress(KeypadEvent key)
{
  const int STOPID_LEN = 5;

  globalState = G_STATE_KEYPAD_INPUT;

  if (key == '#') {
    displayNumber(DISPLAY_DASHES);

    // send to controller
    Serial.println(stopIdBuffer);
    stopIdBufferIndex = stopIdBuffer;

  } else if (key == '*') {
    stopIdBufferIndex = stopIdBuffer;
    displayNumber(DISPLAY_CLEAR);

    // reset cycling routes/minutes
    resetCyclingRoutesMinutes();

    delay(100);
  } else if ((int)stopIdBufferIndex < (int)stopIdBuffer + STOPID_LEN) {
    *stopIdBufferIndex = key;
    *(stopIdBufferIndex+1) = '\0';
    stopIdBufferIndex++;
  } else {
    // oops too many numbers
    stopIdBufferIndex = stopIdBuffer;
    displayNumber(DISPLAY_CLEAR);
    // play error tones
    playErrorTones();

    // reset cycling routes/minutes
    resetCyclingRoutesMinutes();
  }
}

///////////////
///   DTMF  ///
///////////////

void playDTMF(int digit)
{
  freq1.play(DTMF_freq1[digit]);
  freq2.play(DTMF_freq2[digit]);
}

void stopDTMF()
{
  freq1.stop();
  freq2.stop();
}

void playErrorTones()
{
  freq1.play(DTMF_freq1[0], 100);
  freq2.play(DTMF_freq2[0], 100);
  delay(150);
  freq1.play(DTMF_freq1[0], 100);
  freq2.play(DTMF_freq2[0], 100);
  delay(150);
}

///////////////
/// DISPLAY ///
///////////////

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

void displayNumberWithDot(int number, int withDot)
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
  displayRightDigitWithDot(rightDigit, withDot);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void displayNumber(int number)
{
  displayNumberWithDot(number, 0);
}

void displayLeftDigit(int digit)
{
  char byte = byteForNumber(digit);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, byte);
}

void displayRightDigitWithDot(int digit, int withDot)
{
  char byte = byteForNumber(digit);
  if (withDot) {
    byte |= 1 << SEG_DOT;
  }
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, byte);
}

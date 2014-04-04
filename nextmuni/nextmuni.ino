void readSerial();

void displayNumber(int number);
void showNumberWithDuration(int number, unsigned long duration);
void displayLeftDigit(int digit);
void displayRightDigit(int digit);
void lightUpDigit(int DisplayNumber);

int minutes = 99;
char buffer[7];
int buffer_idx = 0;

void setup() {
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  displayNumber(minutes);
  readSerial();
}

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

void readSerial() {
  if (Serial.available() > 0) { // check if there is data waiting
    int inByte = Serial.read(); // read one byte
    if (inByte != 10) { // if byte is not newline
      buffer[buffer_idx] = char(inByte); // just add it to the buffer
      buffer_idx++;
    } else {
      // turn the buffer from string into an integer number
      buffer[buffer_idx] = 0;
      int num = atoi(buffer);
      minutes = num;

      // clean the buffer for the next read cycle
      buffer_idx = 0;

    }

  }
}

///////
///////

void showNumberWithDuration(int number, unsigned long duration) {
 long start = millis();
  while (millis() - start < duration) {
    displayNumber(number); 
  }
}

void displayNumber(int number) {
  int leftDigit = number / 10;
  int rightDigit = number % 10;
  displayLeftDigit(leftDigit);
  displayRightDigit(rightDigit);
}

void displayLeftDigit(int digit) {
  if (digit == 0) {
     //digit = 10; 
  }
   digitalWrite(9, 1);
   digitalWrite(10, 0);
   lightUpDigit(digit);
   delay(5);
}

void displayRightDigit(int digit) {
  if (digit == 0) {
     //digit = 10; 
  }
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  lightUpDigit(digit);
  delay(5);
}

// This function has a case statement 
// which sets the pins high or low,
// and displays each of the digits.

void lightUpDigit(int DisplayNumber) {
  
  switch (DisplayNumber){
    case 0:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 0);
    digitalWrite(7, 1);
    digitalWrite(8, 1);
    break;

    case 1:
    digitalWrite(2, 1);
    digitalWrite(3, 0);
    digitalWrite(4, 1);
    digitalWrite(5, 0);
    digitalWrite(6, 0);
    digitalWrite(7, 0);
    digitalWrite(8, 0);
    break;
    
    
    case 2:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 0);
    digitalWrite(5, 0);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 1);
    break;
    
    case 3:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 0);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 0);
    break;
    
    case 4:
    digitalWrite(2, 1);
    digitalWrite(3, 0);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 0);
    digitalWrite(8, 0);
    break;
    
    case 5:
    digitalWrite(2, 0);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 0);
    break;
    
    case 6:
    digitalWrite(2, 0);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 1);
    break;
    
    case 7:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 0);
    digitalWrite(6, 0);
    digitalWrite(7, 0);
    digitalWrite(8, 0);
    break;
    
    case 8:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 1);
    break;
    
    case 9:
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 0);
    break;
    
    case 10:
    digitalWrite(2, 0);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
    digitalWrite(5, 0);
    digitalWrite(6, 0);
    digitalWrite(7, 0);
    digitalWrite(8, 0);
    break;
  }
}


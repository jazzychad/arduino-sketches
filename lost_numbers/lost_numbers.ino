void showNumberWithDuration(int number, long duration);
void displayNumber(int number);
void displayLeftDigit(int digit);
void displayRightDigit(int digit);
void lightUpDigit(int DisplayNumber);

void setup() {
  // put your setup code here, to run once:
  // Set 7-segement outputs  
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}
int numbers[] = {4, 8, 15, 16, 23, 42, 0};
void loop() {
  // put your main code here, to run repeatedly: 
  
  int i;
  for (i = 0; i < 7; i++) {
    showNumberWithDuration(numbers[i], 800);
    showNumberWithDuration(0, 100); 
  }
  /*
  displayNumber(4);
  delay(1000);
  displayNumber(8);
  delay(1000);
  showNumberWithDuration(15, 1000);
  delay(1000);
  displayNumber(16);
  delay(1000);
  displayNumber(23);
  delay(1000);
  displayNumber(42);
  delay(1000);
  displayNumber(0);
  delay(1000);
  */
}

void showNumberWithDuration(int number, long duration) {
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
     digit = 10; 
  }
   digitalWrite(9, 1);
   digitalWrite(10, 0);
   lightUpDigit(digit);
   delay(5);
}

void displayRightDigit(int digit) {
  if (digit == 0) {
     digit = 10; 
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


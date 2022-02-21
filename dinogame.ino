/*
  LiquidCrystal Library - Dino game

  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * Button to digital pin 6 (for jump)

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

byte Dino[] = {
  B00000,
  B01111,
  B01011,
  B01111,
  B11110,
  B11110,
  B11110,
  B10110
};

byte Cactuis[] = {
  B00000,
  B00000,
  B10110,
  B10110,
  B11110,
  B00110,
  B00110,
  B00110
};

int buttonPressed = 0;

int currentState;

int buttenPin = 6;

int upup = 0;

int enviromentdelay = 100;
int score = 0;
int levelscore = 0;

// What the fuck
int lineOne[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lineTwo[16] = {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(buttenPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Setup done");
  lcd.createChar(1, Dino);
  lcd.createChar(2, Cactuis);
}

void loop() {
  // Button setup
  currentState = digitalRead(buttenPin);
  if(currentState == LOW) {
    buttonPressed = 1;
  } else {
    buttonPressed = 0;
  }

  // Enviornment logic
  long randomNumber = random(0, 10);
  int newVar = 0;
  if(randomNumber == 0) {
    newVar = 4;
  }
  int carTemp = lineTwo[0];
  for(int i = 0; i < (sizeof(lineTwo) / sizeof(lineTwo[0])) - 1; i++) {
    lineTwo[i] = lineTwo[i + 1];
  }
  lineTwo[(sizeof(lineTwo) / sizeof(lineTwo[0])) - 1] = newVar;
  if(lineTwo[0] == 4 and carTemp == 3) {
    Serial.println("Car hit");
    dead();
  }
  lineTwo[0] = carTemp;

  // Jump logic
  if(buttonPressed == 1 and upup == 0) {
    lineOne[0] = 3;
    lineTwo[0] = 0;
    upup = 1;
  }
  else if(upup >= 3) {
    upup = 0;
    lineOne[0] = 0;
    lineTwo[0] = 3;
  } else if(upup < 3 and upup > 0) {
    upup++;
    lineOne[0] = 3;
    lineTwo[0] = 0;
  }
  else {
    lineOne[0] = 0;
    lineTwo[0] = 3;
  }

  // Write to LCD
  lcd.setCursor(0, 0);
  for(int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    //lcd.print(lineOne[i]);
    if(lineOne[i] == 3) {
      lcd.write(byte(1));
    } else if(lineOne[i] == 0) {
      lcd.print(" ");
    } else {
      lcd.print(lineOne[i]);
    }
  }
  for(int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    //lcd.print(lineTwo[i]);
    if(lineTwo[i] == 3) {
      lcd.write(byte(1));
    } else if(lineTwo[i] == 4) {
      lcd.write(byte(2));
    } else if(lineTwo[i] == 0) {
      lcd.print(" ");
    } else {
      lcd.print(lineTwo[i]);
    }
  }

  // Score logic and level logic
  if(levelscore >= 100) {
    levelscore = 0;
    enviromentdelay = enviromentdelay * 0.9;
    Serial.println("Level up");
  }

  score = score + 1;
  levelscore = levelscore + 1;
  
  delay(enviromentdelay);
  Serial.println("Score: " + String(score));
  Serial.println("Level: " + String(enviromentdelay));
}

void dead() { // end screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You died");
  lcd.setCursor(0, 1);
  lcd.print("Score: " + String(score));
  for(int i = 0; i < 16; i++) {
    lineOne[i] = 0;
    lineTwo[i] = 0;
  }
  lineTwo[0] = 3;
  score = 0;
  levelscore = 0;
  delay(200);
  while (1) {
    if (digitalRead(buttenPin) == LOW) {
      break;
    }
  }
}
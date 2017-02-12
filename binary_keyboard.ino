#include <Keyboard.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Set up the pins
const int zeroButton = 6;
const int oneButton = 7;
const int confirmButton = 5;

//We don't want stuff repeating so let's keep track of the state
bool zeroPressed = false;
bool onePressed = false;
bool confirmPressed = false;
bool preview = false;

//better way to do this? Probably but the code is still in development
int binVal = 0;
//Count makes sure we only have 8 bits; no more no less
int count = 7;

void setup()
{
  //Set our inputs
  pinMode(zeroButton, INPUT);
  pinMode(oneButton, INPUT);
  pinMode(confirmButton, INPUT);

  //LCD & Keyboard setup
  Keyboard.begin();
  lcd.begin(20,4); 
  resetLCD();
}


void loop()
{
  //Track button presses
  int zeroState = digitalRead(zeroButton);
  int oneState = digitalRead(oneButton);
  int confirmState = digitalRead(confirmButton);

  //Zero was pressed
  if (zeroState == HIGH && !zeroPressed) {
    if (count >= 0) {
      count--;
      lcd.print("0");
    } else {
      resetLCD();
    }
    zeroPressed = true;
    delay(100);
  } else if (zeroPressed && zeroState != HIGH) { //make sure it doesn't register as two presses
    zeroPressed = false;
  }

  //one was pressed
  if (oneState == HIGH && !onePressed) {
    if (count >= 0) {
      binVal += 1<<count; // 00000001 <-- current count, 2^count
      count--;
      lcd.print("1");
    } else {
      resetLCD();
    }
    onePressed = true;
    delay(100);
  } else if (onePressed && oneState != HIGH) { //make sure it doesn't register as two presses
    onePressed = false;
  }

  if (confirmState == HIGH && !confirmPressed) {
    if (count < 0) {
      Keyboard.write((int)binVal);
      resetLCD();
    }
    confirmPressed = true;
  } else if (confirmState != HIGH && confirmPressed) {
    confirmPressed = false;
  }

  if (count < 0 && !preview) { //Preview the character
    lcd.print(" : ");
    lcd.print((char)(int)binVal); //bin to ascii conversion is easy
    preview = true;
  }
}


void resetLCD() {
  lcd.clear();
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Binary Keyboard V1.0");
  lcd.setCursor(0,1);
  preview = false;
  binVal = 0;
  count = 7;
}

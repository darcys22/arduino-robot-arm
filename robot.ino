#include <LiquidCrystal.h>
#include "buttons.h"

//Each Step is 1.8 degrees, and we are doing 1/16 steps
int stepsPerRevolution = 200*16;
int menuState = 0;
int menuSelectState = LOW;
int buttonState;
int directionState;
int moveState = LOW;
unsigned long lastDisplayed=millis();

//
// Limit Switches
//
const int X_STOP_PIN = 18;
const int Y_STOP_PIN = 19;
const int Z_STOP_PIN = 20;

//
// Steppers
//
const int X_STEP_PIN = 15;
const int X_DIR_PIN = 21;
const int X_ENABLE_PIN = 14;
const int XY_ENABLE_PIN = 14;

const int Y_STEP_PIN = 22;
const int Y_DIR_PIN = 23;
const int Y_ENABLE_PIN = 14;

const int Z_STEP_PIN = 3;
const int Z_DIR_PIN = 2;
const int Z_ENABLE_PIN = 26;

const int E0_STEP_PIN = 1;
const int E0_DIR_PIN = 0;
const int E0_ENABLE_PIN = 14;

const int LCD_PINS_RS = 28;
const int LCD_PINS_ENABLE = 29;
const int LCD_PINS_D4 = 10;
const int LCD_PINS_D5 = 11;
const int LCD_PINS_D6 = 16;
const int LCD_PINS_D7 = 17;
const int ADC_KEYPAD_PIN = 1;

// https://github.com/Klipper3d/klipper/issues/898
// https://user-images.githubusercontent.com/28060331/49003751-e7a15880-f162-11e8-9f89-fd07c6de12ed.png
const int ADC_UP_VAL = 696; //UP
const int ADC_BACK_VAL = 93; // LEFT
const int ADC_ENTER_VAL = 512; // RIGHT
const int ADC_DOWN_VAL = 179; // DOWN
const int ADC_MENU_VAL = 326; // CENTER

const int ADC_DEFAULT_STATE = 1024; // CENTER

LiquidCrystal lcd(LCD_PINS_RS,
                  LCD_PINS_ENABLE,
                  LCD_PINS_D4,
                  LCD_PINS_D5,
                  LCD_PINS_D6,
                  LCD_PINS_D7);

int adc_value = ADC_DEFAULT_STATE;
ADC_buttons display_buttons;

void setup() { 
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(X_STOP_PIN, INPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(XY_ENABLE_PIN, OUTPUT);
  directionState = HIGH;
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(XY_ENABLE_PIN, LOW);
  menuState = 0;
  menuSelectState = LOW;
  moveState = LOW;

  display_buttons.setup_button(ADC_MENU_VAL, [](){
    menuSelectState = menuSelectState ? LOW: HIGH;
  });
  display_buttons.setup_button(ADC_UP_VAL, [](){
    if (menuState < 5)
      menuState += 1;
    else
      menuState = 0;
    menuSelectState = LOW;
  });
  display_buttons.setup_button(ADC_DOWN_VAL, [](){
    if (menuState > 0)
      menuState -= 1;
    else
      menuState = 5;
    menuSelectState = LOW;
  });
  display_buttons.setup_button(ADC_BACK_VAL, [](){
    if (menuSelectState == HIGH)
    {
      directionState = LOW;
      moveState = HIGH;
    }
  });
  display_buttons.setup_button(ADC_ENTER_VAL, [](){
    if (menuSelectState == HIGH)
    {
      directionState = HIGH;
      moveState = HIGH;
    }
  });


  lcd.clear();
  lcd.begin(16,2);
  lcd.print("SD Robot Arm");
  delay(3000);

}

void displayMenu() {
  if (millis() - lastDisplayed > 1000)
  {
    lcd.clear();
    if (menuState == 0)
    {
      lcd.print("1. Base");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    if (menuState == 1)
    {
      lcd.print("2. Lower Arm");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    if (menuState == 2)
    {
      lcd.print("3. Upper Arm");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    if (menuState == 3)
    {
      lcd.print("4. Wrist");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    if (menuState == 4)
    {
      lcd.print("5. Finger");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    if (menuState == 5)
    {
      lcd.print("6. End");
      if (menuSelectState == HIGH)
      {
        lcd.print(" - Selected");
      }
    }
    lastDisplayed = millis();
  }
}

void loop() { 
  adc_value = analogRead(ADC_KEYPAD_PIN);
  display_buttons.handle_button(millis(), adc_value);

  displayMenu();
  
  digitalWrite(X_DIR_PIN, directionState);
  int button_state = digitalRead(X_STOP_PIN);
  if (button_state == LOW)
  {
    for (int i = 0; i < stepsPerRevolution; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(500);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(500);
    }
    directionState = directionState ? LOW: HIGH;
  }
  if (moveState == HIGH)
  {
    for (int i = 0; i < stepsPerRevolution; i++)
    {
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(500);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(500);
    }
    moveState = LOW;
  }
}

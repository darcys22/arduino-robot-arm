#include <LiquidCrystal.h>

//Each Step is 1.8 degrees, and we are doing 1/16 steps
int stepsPerRevolution = 200*16;
int menuState = 0;
int menuSelectState = LOW;
int buttonState;
int directionState;
int moveState = LOW;
int adc_value;
unsigned long lastRead=millis();
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

const int ADC_UP_VAL = 700;
const int ADC_BACK_VAL = 100;
const int ADC_ENTER_VAL = 500;
const int ADC_DOWN_VAL = 200;
const int ADC_MENU_VAL = 300;

LiquidCrystal lcd(LCD_PINS_RS,
                  LCD_PINS_ENABLE,
                  LCD_PINS_D4,
                  LCD_PINS_D5,
                  LCD_PINS_D6,
                  LCD_PINS_D7);


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

  lcd.clear();
  lcd.begin(16,2);
  lcd.print("SD Robot Arm");
  delayMicroseconds(5000);
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
  adc_value = 0;

  for (size_t i = 0; i < 20; i++)
  {
    adc_value += analogRead(ADC_KEYPAD_PIN);
  }
  adc_value = adc_value/20;
/*   if (millis()-lastRead>500)
  {
    lcd.clear();
    lcd.begin(16,2);
    lcd.print(adc_value);
    lastRead = millis();
  } */

  if ((adc_value>ADC_MENU_VAL-50)&&(adc_value<ADC_MENU_VAL+50)&&(millis()-lastRead>500))
  {
    menuSelectState = menuSelectState ? LOW: HIGH;
    lastRead = millis();
  }
  else if ((adc_value>ADC_UP_VAL-50)&&(adc_value<ADC_UP_VAL+50)&&(millis()-lastRead>500))
  {
    if (menuState < 6)
      menuState += 1;
    else
      menuState = 5;
    menuState %= 6;
    menuSelectState = LOW;
    lastRead = millis();
  }
  else if ((adc_value>ADC_BACK_VAL-50)&&(adc_value<ADC_BACK_VAL+50)&&(millis()-lastRead>500))
  {
    if (menuSelectState == HIGH)
    {
      directionState = LOW;
      moveState = HIGH;
      lastRead = millis();
    }

  }
  else if ((adc_value>ADC_ENTER_VAL-50)&&(adc_value<ADC_ENTER_VAL+50)&&(millis()-lastRead>500))
  {
    if (menuSelectState == HIGH)
    {
      directionState = HIGH;
      moveState = HIGH;
      lastRead = millis();
    }
  }
  else if ((adc_value>ADC_DOWN_VAL-50)&&(adc_value<ADC_DOWN_VAL+50)&&(millis()-lastRead>500))
  {
    if (menuState > 0)
      menuState -= 1;
    else
      menuState = 5;
    menuState %= 6;
    menuSelectState = LOW;
    lastRead = millis();
  }

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

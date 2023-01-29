#include <LiquidCrystal.h>

//Each Step is 1.8 degrees, and we are doing 1/16 steps
int stepsPerRevolution = 200*16;
int buttonState;
int directionState;
int adc_value;
unsigned long lastRead=millis();

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

//TODO sean delete this
char snum[5];

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

  lcd.clear();
  lcd.begin(16,2);
  lcd.print("hello world");

}

void loop() { 
  adc_value = 0;

  for (size_t i = 0; i < 40; i++)
  {
    adc_value += analogRead(ADC_KEYPAD_PIN);
  }
  adc_value = adc_value/40;
  if (millis()-lastRead>500)
  {
    lcd.clear();
    lcd.begin(16,2);
    if (adc_value > 0)
    {
      lcd.print("hello ");
      itoa(adc_value, snum, 10);
      lcd.print(snum);
    } else {
      lcd.print("nothing pressed");
    }
    lastRead = millis();
  }


/*   if ((adc_value>VAL1-50)&&(adc_value<VAL1+50)&&(millis()-lastRead>300)
  {
    lcd.clear();
    lcd.begin(16,2);
    lcd.print("hello");
  } */
  
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
}

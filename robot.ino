

int delay_value = 1000;
int button_pin = 18;
int dirPin = 21;
int stepPin = 15;
int xyEnablePin = 14;

//Each Step is 1.8 degrees, and we are doing 1/16 steps
int stepsPerRevolution = 200*16;
int buttonState;
int directionState;

void setup() { 
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(button_pin, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(xyEnablePin, OUTPUT);
  directionState = HIGH;
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(xyEnablePin, LOW);
}

void loop() { 
  digitalWrite(dirPin, directionState);
  int button_state = digitalRead(button_pin);
  if (button_state == LOW)
  {
    for (int i = 0; i < stepsPerRevolution; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
    directionState = directionState ? LOW: HIGH;
  }
}

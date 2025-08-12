#include <Servo.h>

Servo servoRPM;
const int rpmPin = 9;
Servo servoSpeed;
const int speedPin = 10;

const int digitPin = 13;
const int SEG_A = 2;
const int SEG_B = 3;
const int SEG_C = 4;
const int SEG_D = 5;
const int SEG_E = 6;
const int SEG_F = 7;
const int SEG_G = 8;

const int ledPin = 12;

const long blinkInterval = 100;
long lastBlinkTime = 0;
int ledState = LOW;

bool engineIsRunning = false;

float rpm = 1800.0;
int speed = 0;
int gear = 1;
int throttleValue = -1;

const float gearRatios[] = {1.0, 0.7, 0.49, 0.343, 0.2401, 0.16807};

const float THROTTLE_CONSTANT = 60.0;
const float OVERDRIVE_CONSTANT = 0.0031;
const float UPSHIFT_RPM_MULTIPLIER = 0.7;
const float DOWNSHIFT_RPM_MULTIPLIER = 1.42857;

void clearDisplay();
void displayZero();
void displayOne();
void displayTwo();
void displayThree();
void displayFour();
void displayFive();
void displaySix();
void displaySeven();
void displayEight();
void displayNine();
void startupSequence();

void setup() {
  Serial.begin(9600);

  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  pinMode(digitPin, OUTPUT);
  digitalWrite(digitPin, HIGH);

  pinMode(ledPin, OUTPUT);
  
  servoRPM.attach(rpmPin);
  servoSpeed.attach(speedPin);

  startupSequence();
}

void loop() {
  if (Serial.available() > 0) {
    throttleValue = Serial.parseInt();
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  if (throttleValue >= 0) {
    engineIsRunning = true;
  } else {
    engineIsRunning = false;
  }

  if (engineIsRunning) {
    if (gear == 0) {
      gear = 1;
    }

    int mappedGas = constrain(map(throttleValue, 0, 255, -87, 122), -87, 122);
    rpm += (float)(mappedGas * gearRatios[gear - 1] * THROTTLE_CONSTANT) / 100.0;
    rpm = constrain(rpm, 1800, 10000);

    if (rpm > 7500) {
      if (millis() - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = millis();
        
        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }
        
        digitalWrite(ledPin, ledState);
      }
    } else if (rpm >= 6500) {
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
    } else {
      digitalWrite(ledPin, LOW);
      ledState = LOW;
    }

    if (rpm >= 8000 && gear < 6) {
      gear++;
      rpm *= UPSHIFT_RPM_MULTIPLIER;
    } else if (rpm <= 4000 && gear > 1) {
      gear--;
      rpm *= DOWNSHIFT_RPM_MULTIPLIER;
    }

    speed = rpm / gearRatios[gear - 1] * OVERDRIVE_CONSTANT;

    int rpmAngle = map(rpm, 0, 8000, 180, 0);
    servoRPM.write(rpmAngle);

    int speedAngle = map(speed, 0, 300, 180, 0);
    servoSpeed.write(speedAngle);

    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print(" | Speed: ");
    Serial.print(speed);
    Serial.print(" | Gear: ");
    Serial.println(gear);

    switch (gear) {
      case 0:
        displayZero();
        break;
      case 1:
        displayOne();
        break;
      case 2:
        displayTwo();
        break;
      case 3:
        displayThree();
        break;
      case 4:
        displayFour();
        break;
      case 5:
        displayFive();
        break;
      case 6:
        displaySix();
        break;
      default:
        clearDisplay();
        break;
    }
  } else {
    rpm = 0;
    speed = 0;
    gear = 0;
    servoRPM.write(180);
    servoSpeed.write(180);
    displayZero();
    Serial.println("Engine is off.");
  }
}

void startupSequence() {
  delay(1000);
  Serial.println("Starting servo sweep 180-0");
  for (int angle = 180; angle >= 0; angle -= 1) {
    servoRPM.write(angle);
    servoSpeed.write(angle);
    delay(5);
  }
  delay(200);
  
  Serial.println("Starting servo sweep 0-180");
  for (int angle = 0; angle <= 180; angle += 1) {
    servoRPM.write(angle);
    servoSpeed.write(angle);
    delay(5);
  }

  digitalWrite(ledPin, HIGH);    
  Serial.println("Starting display count up 0-9");
  for (int i = 0; i <= 9; i++) {
    switch (i) {
      case 0: displayZero(); break;
      case 1: displayOne(); break;
      case 2: displayTwo(); break;
      case 3: displayThree(); break;
      case 4: displayFour(); break;
      case 5: displayFive(); break;
      case 6: displaySix(); break;
      case 7: displaySeven(); break;
      case 8: displayEight(); break;
      case 9: displayNine(); break;
    }
    delay(100);
  }
  delay(100);

  Serial.println("Starting display count down 9-1");
  for (int i = 9; i >= 1; i--) {
    switch (i) {
      case 0: displayZero(); break;
      case 1: displayOne(); break;
      case 2: displayTwo(); break;
      case 3: displayThree(); break;
      case 4: displayFour(); break;
      case 5: displayFive(); break;
      case 6: displaySix(); break;
      case 7: displaySeven(); break;
      case 8: displayEight(); break;
      case 9: displayNine(); break;
    }
    delay(100);
  }
  digitalWrite(ledPin, LOW);
}

void clearDisplay() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}

void displayZero() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
}

void displayOne() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}

void displayTwo() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
}

void displayThree() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
}

void displayFour() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displayFive() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displaySix() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displaySeven() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}

void displayEight() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void displayNine() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

#include <Servo.h>

#define LEFT_1  D5
#define LEFT_2  D6
#define RIGHT_1  D3
#define RIGHT_2  D4
#define SERVO_PIN D0
#define ECHO_PIN D2
#define TRIG_PIN D1
#define LEFT_LIGHT  D7
#define RIGHT_LIGHT  D8

Servo myservo;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEFT_1, OUTPUT);
  pinMode(LEFT_2, OUTPUT);
  pinMode(RIGHT_1, OUTPUT);
  pinMode(RIGHT_2, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(LEFT_LIGHT, OUTPUT);
  pinMode(RIGHT_LIGHT, OUTPUT);

  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  turnEyes(93);

  blinkLED(2);
}

void loop() {

  if (getDistance() <= 30) {
    stopCar();
    turnToFreeArea();
  } else {
    goForward();
  }

}

void turnToFreeArea() {
  delay(1000);
  long straightDistance = getDistance();
  turnEyes(48);
  delay(1000);
  long rightDistance = getDistance();
  turnEyes(138);
  delay(1000);
  long leftDistance = getDistance();
  turnEyes(93);
  delay(1000);

  if(leftDistance > rightDistance) {
    //can turnLeft
    if(leftDistance > 30) {
      //if obstacles in left far from 30cm
      turnLeft();
      delay(500);
      stopCar();
    }
  } else {
    //can turn right
    if(rightDistance > 30) {
      //if obstacles in right far from 30cm
      turnRight();
      delay(500);
      stopCar();
    }
  }
}

void goForward() {
  digitalWrite(LEFT_1, HIGH);
  digitalWrite(LEFT_2, LOW);
  digitalWrite(RIGHT_1, HIGH);
  digitalWrite(RIGHT_2, LOW);
}

void goBackward() {
  digitalWrite(LEFT_1, LOW);
  digitalWrite(LEFT_2, HIGH);
  digitalWrite(RIGHT_1, LOW);
  digitalWrite(RIGHT_2, HIGH);
}

void turnLeft() {
  digitalWrite(LEFT_1, LOW);
  digitalWrite(LEFT_2, HIGH);
  digitalWrite(RIGHT_1, HIGH);
  digitalWrite(RIGHT_2, LOW);
}

void turnRight() {
  digitalWrite(LEFT_1, HIGH);
  digitalWrite(LEFT_2, LOW);
  digitalWrite(RIGHT_1, LOW);
  digitalWrite(RIGHT_2, HIGH);
}

void stopCar() {
  digitalWrite(LEFT_1, LOW);
  digitalWrite(LEFT_2, LOW);
  digitalWrite(RIGHT_1, LOW);
  digitalWrite(RIGHT_2, LOW);
}

uint16_t getDistance() {
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  uint16_t duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  uint16_t distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}

void blinkLED(uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(RIGHT_LIGHT, HIGH);
    digitalWrite(LEFT_LIGHT, HIGH);
    delay(1000);
    digitalWrite(RIGHT_LIGHT, LOW);
    digitalWrite(LEFT_LIGHT, LOW);
    delay(1000);
  }
}

void turnEyes(int degree) {
  myservo.write(degree);
}

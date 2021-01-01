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
#define FRONT_DEGREE  93
#define RIGHT_DEGREE  48
#define LEFT_DEGREE 138
#define DISTANCE_THRESHOLD  30

Servo myservo;
uint16_t straightDistance;
uint16_t rightDistance;
uint16_t leftDistance;

void setup() {
  //Set all required pin modes to OUTPUT and INPUT
  pinMode(LEFT_1, OUTPUT);
  pinMode(LEFT_2, OUTPUT);
  pinMode(RIGHT_1, OUTPUT);
  pinMode(RIGHT_2, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(LEFT_LIGHT, OUTPUT);
  pinMode(RIGHT_LIGHT, OUTPUT);

  //Attach servo control data pin
  myservo.attach(SERVO_PIN);

  //Adjust servo to face front
  turnEyes(FRONT_DEGREE);

  //Wait for two seconds
  blinkLED(2, 1000);
}

void loop() {

  //In the loop it always measure the distance to font obstacle and stop if found and check for a free area to turn and move forward again
  if (getDistance() <= DISTANCE_THRESHOLD) {
    stopCar();
    turnToFreeArea();
  } else {
    goForward();
  }

}

void measureDistance() {
  //Wait for 1 second
  blinkLED(2, 250);

  //Get distance to obstacle in straight
  straightDistance = getDistance();

  //Turn eyes to right
  turnEyes(RIGHT_DEGREE);

  //Wait for 1 second
  blinkLED(2, 250);

  //Get distance to obstacle in right
  rightDistance = getDistance();

  //Turn eyes to left
  turnEyes(LEFT_DEGREE);

  //Wait for 1 second
  blinkLED(2, 250);

  //Get distance to obstacle in left
  leftDistance = getDistance();

  //Turn eyes to front again
  turnEyes(FRONT_DEGREE);

  //Wait for 1 second
  blinkLED(2, 250);
}

void turnToFreeArea() {

  measureDistance();

  //If obstacle in front is too close, go backward a bit and measure surrounding obstacle distance again
  if (straightDistance <= 10) {
    goBackward();
    blinkLED(random(1, 5), 100);
    stopCar();
    measureDistance();
  }

  //Distance to obstacles in left is greater than obstacles in right. So can turn to left
  if (leftDistance > rightDistance) {

    //Check distance to left obstacle is greater than threshold (30cm). If so turn left and if not go backward for few distance and recheck
    if (leftDistance > DISTANCE_THRESHOLD) {
      turnLeft();
      blinkLED(1, 250);
      stopCar();
    } else {
      goBackward();
      blinkLED(random(1, 5), 100);
      stopCar();
    }
  }

  //Distance to obstacles in right is greater than obstacles in left. So can turn to right
  else if (rightDistance > leftDistance) {

    //Check distance to right obstacle is greater than threshold (30cm). If so turn right and if not go backward for few distance and recheck
    if (rightDistance > DISTANCE_THRESHOLD) {
      turnRight();
      blinkLED(1, 250);
      stopCar();
    } else {
      goBackward();
      blinkLED(random(1, 5), 100);
      stopCar();
    }
  }

  //Distance to lleft and right obstacles are almost equal. So go backward for few distance and recheck
  else {
    goBackward();
    blinkLED(random(1, 5), 100);
    stopCar();
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

void blinkLED(uint8_t count, uint16_t _delay) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(RIGHT_LIGHT, HIGH);
    digitalWrite(LEFT_LIGHT, HIGH);
    delay(_delay);
    digitalWrite(RIGHT_LIGHT, LOW);
    digitalWrite(LEFT_LIGHT, LOW);
    delay(_delay);
  }
}

void turnEyes(uint16_t degree) {
  myservo.write(degree);
}

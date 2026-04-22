//libraries installation
#include <Servo.h>
#include <RedBot.h>

// Dc motors pins
#define PINA_Left 3
#define PINB_Left 4
#define PINA_Right 2
#define PINB_Right 9

//IR Sesnor - Line Following
RedBotSensor IRSensorLeft = RedBotSensor(A11);
RedBotSensor IRSensorRight = RedBotSensor(A12);
int rightSensor = 0;
int leftSensor = 0;

// Servo motor pins
#define pinServo1 23
#define pinServo2 31

// Ultrasonic sensor pins
#define trigPinFront 36
#define echoPinFront 38
#define trigPinRight 51
#define echoPinRight 53

// Declare a servo motor class
Servo servo1;
Servo servo2;

// Define color thresholds
const int RED_THRESHOLD = 140;
const int YELLOW_THRESHOLD = 140;
const int WHITE_THRESHOLD = 100;
const int BLACK_THRESHOLD = 850;

bool blackLineCrossed = false;
bool firstRotation = false;
bool followLine = false;
bool finishedLine = false;

// Servo motors flag variables that help control position and speed
bool servo1Position = false;
bool servo2Position = false;
bool servo1FinalPosition = false;
bool servo2FinalPosition = false;

// Left DC motor
volatile long enc_count_Left;
volatile int wheel_angle_Left;
const int pinAI1_Left = 11; // Pin allocation for AI1
const int pinAI2_Left = 12; // Pin allocation for AI2
const int pinPWMA = 13; // Pin allocation for the PWMA pin
boolean AI1_left = 0; // AI1 pin value
boolean AI2_left = 0; // AI2 pin value
boolean standByA = 0; // standByA pin Value
boolean rotDirect_Left = 0; // Rotation direction variable left
unsigned char PWMAValue_Left = 0; // PWMA value to be written to the output

// right DC motor
volatile long enc_count_Right;
volatile int wheel_angle_Right;
const int pinBI1_Right = 7; // Pin allocation for BI1
const int pinBI2_Right = 6; // Pin allocation for BI2
const int pinPWMB = 5; // Pin allocation for the PWMB pin
boolean BI1_Right = 0; // BI1 pin value
boolean BI2_Right = 0; // BI2 pin value
boolean standByB = 0; // standByB pin Value
boolean rotDirect_Right = 0; // Rotation direction variable right
unsigned char PWMBValue_Right = 0; // PWMB value to be written to the output


//Standby
const int pinstandBy = 10; // Pin allocation for the  A pin
boolean standBy = 0; // standBy pin Value


void setup() {
  pinMode(pinstandBy, OUTPUT);
  standBy = true;
  digitalWrite(pinstandBy, standBy);
  Serial.begin(9600);

  // LEFT SETUP for DC and ENCODER
  pinMode(PINA_Left, INPUT);
  pinMode(PINB_Left, INPUT);
  attachInterrupt(digitalPinToInterrupt(PINA_Left), channelA1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PINB_Left), channelB1, CHANGE);
  enc_count_Left = 0;
  pinMode(pinAI1_Left, OUTPUT);
  pinMode(pinAI2_Left, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  AI1_left = false;
  AI2_left = true;
  PWMAValue_Left = 100;

  // right SETUP for DC and ENCODER
  pinMode(PINA_Right, INPUT);
  pinMode(PINB_Right, INPUT);
  attachInterrupt(digitalPinToInterrupt(PINA_Right), channelA2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PINB_Right), channelB2, CHANGE);
  enc_count_Right = 0;
  pinMode(pinBI1_Right, OUTPUT);
  pinMode(pinBI2_Right, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  BI1_Right = false;
  BI2_Right = true;
  PWMBValue_Right = 100;

  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  //servo motor initial positions
  servo2.attach(pinServo2);
  servo2.write(150);
  blackLineCrossed = false;

}

void loop() {

  int rightDistance = measureDistance(trigPinRight, echoPinRight);
  int frontDistance = measureDistance(trigPinFront, echoPinFront);

  rotate180();
  tunnel1();

 }


void rotate180() {
  Serial.println("Rotate Right");
  while(measureDistance(trigPinRight, echoPinRight) > 10) {
    delay(100);
    Serial.println(measureDistance(trigPinRight, echoPinRight));
    turnRight();
  }
  Serial.println(measureDistance(trigPinRight, echoPinRight));

  stop();
}


void tunnel1() {
  Serial.println("Tunnel");

  while (measureDistance(trigPinFront, echoPinFront) > 10){
    moveForward();
    blackLineCrossed = true;
  }
  stop();
  while(blackLineCrossed){
    stop();
    Serial.print("I crossed the line: ");
    while(IRSensorRight.read() < BLACK_THRESHOLD){
      turnLeft();
    }
    followLine = true;
    blackLineCrossed = false;
    stop();
  }

  while(followLine){
    stop();
    Serial.println("Doing line following");
    while(measureDistance(trigPinFront, echoPinFront) > 10) {         // change distance suitable for board distance 
        followBlackLine();
    }
    stop();
    followLine = false;
  }

}

//Function for Arm to hit the board

//Returning After arm task
void comingBack(){

  Serial.println("Returning to original position");
  while(measureDistance(trigPinRight, echoPinRight) > 10) {  //Change condition based on situation to come out of loop
      followRedLine();
  }
  stop();
  
}


// function to move forward
void moveForward() {
  // ---- Left Wheel ----
  AI1_left = true;
  AI2_left = false;
  digitalWrite(pinAI1_Left, AI1_left);
  digitalWrite(pinAI2_Left, AI2_left);
  analogWrite(pinPWMA, 150);

  // ---- Right Wheel ----
  BI1_Right = true;
  BI2_Right = false;
  digitalWrite(pinBI1_Right, BI1_Right);
  digitalWrite(pinBI2_Right, BI2_Right);
  analogWrite(pinPWMB, 135);

}

// funtion to move backwards
void moveBackwards() {
  // ---- Left Wheel ----
  AI1_left = false;
  AI2_left = true;
  digitalWrite(pinAI1_Left, AI1_left);
  digitalWrite(pinAI2_Left, AI2_left);
  analogWrite(pinPWMA, PWMAValue_Left);


  // ---- Right Wheel ----
  BI1_Right = false;
  BI2_Right = true;
  digitalWrite(pinBI1_Right, BI1_Right);
  digitalWrite(pinBI2_Right, BI2_Right);
  analogWrite(pinPWMB, PWMBValue_Right);
}

//function for the robot to turn left
void turnLeft() {
  // ---- Left Wheel ----
  AI1_left = false;
  AI2_left = true;
  digitalWrite(pinAI1_Left, AI1_left);
  digitalWrite(pinAI2_Left, AI2_left);
  analogWrite(pinPWMA, 50);

  // ---- Right Wheel ----
  BI1_Right = true;
  BI2_Right = false;
  digitalWrite(pinBI1_Right, BI1_Right);
  digitalWrite(pinBI2_Right, BI2_Right);
  analogWrite(pinPWMB, 50);

}

//function for the robot to turn right
void turnRight() {
 
  // ---- Left Wheel ----
  AI1_left = true;
  AI2_left = false;
  digitalWrite(pinAI1_Left, AI1_left);
  digitalWrite(pinAI2_Left, AI2_left);
  analogWrite(pinPWMA, 100);


  // ---- Right Wheel ----
  BI1_Right = false;
  BI2_Right = true;
  digitalWrite(pinBI1_Right, BI1_Right);
  digitalWrite(pinBI2_Right, BI2_Right);
  analogWrite(pinPWMB, 100);
}

//function for the robot to stop moving
void stop() {

  // ---- Left Wheel ----
  AI1_left = false;
  AI2_left = false;
  digitalWrite(pinAI1_Left, AI1_left);
  digitalWrite(pinAI2_Left, AI2_left);
  analogWrite(pinPWMA, PWMAValue_Left);


  // ---- Right Wheel ----
  BI1_Right = false;
  BI2_Right = false;
  digitalWrite(pinBI1_Right, BI1_Right);
  digitalWrite(pinBI2_Right, BI2_Right);
  analogWrite(pinPWMB, PWMBValue_Right);
}

void channelA1() {
  //  //Serial.print("in a\n");
  if (digitalRead(PINA_Left)) {
    if (digitalRead(PINB_Left)) {
      enc_count_Left--;
    } else {
      enc_count_Left++;
    }
  } else {
    if (digitalRead(PINB_Left)) {
      enc_count_Left++;
    } else {
      enc_count_Left--;
    }
  }
  wheel_angle_Left = int(enc_count_Left * 11.25) % 360;
}

void channelB1() {
  //  //Serial.print("in b\n");
  if (digitalRead(PINB_Left)) {
    if (digitalRead(PINA_Left)) {
      enc_count_Left++;
    } else {
      enc_count_Left--;
    }
  } else {
    if (digitalRead(PINA_Left)) {
      enc_count_Left--;
    } else {
      enc_count_Left++;
    }
  }
  wheel_angle_Left = int(enc_count_Left * 11.25) % 360;
}

//RIGHT ENCODER COUNTING
void channelA2() {
  //  //Serial.print("in a\n");
  if (digitalRead(PINA_Right)) {
    if (digitalRead(PINB_Right)) {
      enc_count_Right--;
    } else {
      enc_count_Right++;
    }
  } else {
    if (digitalRead(PINB_Right)) {
      enc_count_Right++;
    } else {
      enc_count_Right--;
    }
  }
  wheel_angle_Right = int(enc_count_Right * 11.25) % 360;
}

void channelB2() {
  //  //Serial.print("in b\n");
  if (digitalRead(PINB_Right)) {
    if (digitalRead(PINA_Right)) {
      enc_count_Right++;
    } else {
      enc_count_Right--;
    }
  } else {
    if (digitalRead(PINA_Right)) {
      enc_count_Right--;
    } else {
      enc_count_Right++;
    }
  }
  wheel_angle_Right = int(enc_count_Right * 11.25) % 360;
}

//get the distance from the ultrasonic sensors
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void followRedLine() {

  rightSensor = IRSensorLeft.read();
  leftSensor = IRSensorRight.read();

  if (rightSensor > RED_THRESHOLD && leftSensor < WHITE_THRESHOLD) {
    turnRight();
  }

  if (leftSensor > RED_THRESHOLD && rightSensor < WHITE_THRESHOLD) {
    turnLeft();
  }

  if (leftSensor < WHITE_THRESHOLD && rightSensor < WHITE_THRESHOLD) {
    moveForward();
  }

}

void followRedLine2() {

  rightSensor = IRSensorLeft.read();
  leftSensor = IRSensorRight.read();

  if (rightSensor > RED_THRESHOLD) {
    turnRight();
  } else if (leftSensor > RED_THRESHOLD) {
    turnLeft();
  } else {
    moveForward();
  }

}

void followBlackLine() {

  rightSensor = IRSensorLeft.read();
  leftSensor = IRSensorRight.read();

  if (rightSensor > BLACK_THRESHOLD && leftSensor < YELLOW_THRESHOLD) {
    turnRight();
  }

  if (leftSensor > BLACK_THRESHOLD && rightSensor < YELLOW_THRESHOLD) {
    turnLeft();
  }

  if (leftSensor < YELLOW_THRESHOLD && rightSensor < YELLOW_THRESHOLD) {
    moveForward();
  }


}

void followBlackLine2() {
  Serial.println(" I am following the line");
  
  leftSensor = IRSensorLeft.read();
  rightSensor = IRSensorRight.read();
  Serial.println(rightSensor);
  if(measureDistance(trigPinFront, echoPinFront) < 13){
    stop();
    delay(1000);

    if (!servo1FinalPosition ) {
        servo2.write(0);
        delay(1000);
        finishedLine = true;

    }

    servo2FinalPosition= true;
    servo1FinalPosition = true;
  }
 else if (rightSensor > BLACK_THRESHOLD) {
    turnRight();
  } else if (leftSensor > BLACK_THRESHOLD) {
    turnLeft();
  } else {
    moveForward();
  }

}

void initialArmPosition() {

  if (!servo2Position) {
    for (int i = 0; i < 180; i++) {
      servo2.write(i);
      delay(10);

    }

  }
  servo2Position = true;

  if (!servo1Position) {
    for (int i = 100; i >= 50; i--) {

      servo1.write(i);
      delay(10);

    }

  }
  servo1Position = true;

}

void finalArmPosition1() {

  if (!servo1FinalPosition) {

    for (int j = 50; j <= 120; j++) {
      servo1.write(j);
      delay(10);

    }

  }

  servo1FinalPosition = true;

}

void finalArmPosition2() {

  if (!servo2FinalPosition) {

    for (int j = 180; j > 0; j--) {
      servo2.write(j);
      delay(10);

    }

  }

  servo2FinalPosition = true;

}

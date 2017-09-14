#include <Servo.h>

#define ServoPIN_R  10
#define ServoPIN_L  11
#define photoPin_R  0
#define photoPin_L  1
#define photoPin_up  4

//Wheels
Servo leftServo;  
Servo rightServo;

//Ping Sensor
const int frontRightSense = 7;
const int frontLeftSense = 9;
const int rightSense = 8;
const int leftSense = 6;

int stopL = 1890;
int stopR = 1810;

//Light Sensor
int lightLevel_R;
int lightLevel_L;
int lightLevel_up;

//IR Sensor
int sensorPin = 3; //ananlog pin 0
int sensorPin2 = 2;

//State robot is in(what it is attempting to achieve
int state = 3;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  //Wheels
  leftServo.attach(ServoPIN_L);  // attaches the servo on pin 11 to the servo object 
  rightServo.attach(ServoPIN_R);  // attaches the servo on pin 10 to the servo object 
  
  Serial.begin(9600);

  //Setup the starting light level limits
  lightLevel_R =analogRead(photoPin_R);
  lightLevel_L =analogRead(photoPin_L);
  lightLevel_up=analogRead(photoPin_up);
}//--(end setup )---

//MAIN LOOP___________________________________________________________________________________________


void loop() {
  // define ping sensors
    long frontRightDuration, frontRightcm, frontLeftDuration, frontLeftcm, rightDuration, rightcm, leftDuration, leftcm;

  //*****PING READS******
  // frontRight Ping
  pinMode(frontRightSense, OUTPUT);
  digitalWrite(frontRightSense, LOW);
  delayMicroseconds(2);
  digitalWrite(frontRightSense, HIGH);
  delayMicroseconds(5);
  digitalWrite(frontRightSense, LOW);
  pinMode(frontRightSense, INPUT);
  frontRightDuration = pulseIn(frontRightSense, HIGH);

  //Right Ping
    pinMode(rightSense, OUTPUT);
    digitalWrite(rightSense, LOW);
    delayMicroseconds(2);
    digitalWrite(rightSense, HIGH);
    delayMicroseconds(5);
    digitalWrite(rightSense, LOW);
    pinMode(rightSense, INPUT);
    rightDuration = pulseIn(rightSense, HIGH);

  //frontLeft Ping
  pinMode(frontLeftSense, OUTPUT);
  digitalWrite(frontLeftSense, LOW);
  delayMicroseconds(2);
  digitalWrite(frontLeftSense, HIGH);
  delayMicroseconds(5);
  digitalWrite(frontLeftSense, LOW);
  pinMode(frontLeftSense, INPUT);
  frontLeftDuration = pulseIn(frontLeftSense, HIGH);

  //Left Ping
  if (state == 0 || state ==2){
    pinMode(leftSense, OUTPUT);
    digitalWrite(leftSense, LOW);
    delayMicroseconds(2);
    digitalWrite(leftSense, HIGH);
    delayMicroseconds(5);
    digitalWrite(leftSense, LOW);
    pinMode(leftSense, INPUT);
    leftDuration = pulseIn(leftSense, HIGH);
  }

  frontRightcm = microsecondsToCentimeters(frontRightDuration);
  //Serial.print(frontRightcm);
  //Serial.print("cm, ");

  rightcm = microsecondsToCentimeters(rightDuration);
  //Serial.print(rightcm);
  //Serial.print("cm, ");

  frontLeftcm = microsecondsToCentimeters(frontLeftDuration);
  //Serial.print(frontLeftcm);
  //Serial.print("cm");

  leftcm = microsecondsToCentimeters(leftDuration);
  //Serial.println();
  //******END PING******

  //******UPPER LIGHT SENSOR******
  lightLevel_up=analogRead(photoPin_up);
  //******END LIGHT SENSOR******

  //******IR SENSOR******
  int valR = analogRead(sensorPin2);
  int val = analogRead(sensorPin);
  Serial.println(valR);
  //******END IR SENSOR******

  
  //Test State
  if(state == 111){
    turnLeft90();
    delay(2000);
  }


  //Initial state for class Demo
  if(0 == state) {

    //Left Sensor
    /*
    if(leftcm < rightcm){
      turnRight90();
      stop();
      state = 1;
    }
    else{
      turnLeft90();
      stop();
      state = 2;
    }
    */

    //No Left Sensor
    stop();
    if(frontRightcm < 5){
      state = 3;
    }
  }

  //Direct Approach right side wall*****
  else if(1 == state && (frontRightcm < 30 || val < 400)) {
    turnLeft90();
    slowForward();
    state = 3;
  }

  //Direct Approach left side wall*****
  else if(2 == state && (frontLeftcm < 30 || val < 400)) {
    turnRight90();
    slowForward();
    state = 4;
  }

  //Right Wall Riding****
  else if(3 == state){ 
    if(lightLevel_up <10 ){
      room();
    }
    else if(val < 400){
      stop();
      slowBackward();
      delay(1000);
      slowLeft();
    }
    else if(frontRightcm < 30){
      slowBackward();
    }
    else if(frontRightcm <35 && frontLeftcm <35){
      normLeft();
    }
    else if(frontRightcm <45){
      normLeft();
    }
    else if(rightcm > 40){
      slowRight();
    }
    else if(rightcm <25){
      slowLeft();
    }
    else{
      slowForward();
    }
  }  

  //Left Wall Riding****
  else if(4 == state){ 
    if(lightLevel_up <10 ){
      room();
    }
    else if(val < 400){
      stop();
      slowBackward();
      delay(1000);
      slowRight();
    }
    else if(frontLeftcm < 7){
      slowBackward();
    }
    else if(frontLeftcm <45 && leftcm <45){
      turnRight90();
    }
    else if(frontLeftcm <45){
      normRight();
    }
    else if(leftcm > 40){
      slowLeft();
    }
    else if(leftcm <25){
      slowRight();
    }
    else{
      slowForward();
    }
  }
}

//DEFINITIONS_________________________________________________________________________________
long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void forward() {
  leftServo.writeMicroseconds(2000);
  rightServo.writeMicroseconds(2000);
  delay(1000);
}

void backward() {
  leftServo.writeMicroseconds(1000);
  rightServo.writeMicroseconds(1000);
  delay(1000);
}

void slowForward() {
  leftServo.writeMicroseconds(stopL+100);
  rightServo.writeMicroseconds(stopR+110);
}

void slowBackward() {
  leftServo.writeMicroseconds(stopL-100);
  rightServo.writeMicroseconds(stopR-100);
  delay(1000);
}

void turnRight90(){
  leftServo.writeMicroseconds(stopL+100);
  rightServo.writeMicroseconds(stopR-100);
  delay(1530);
  stop();
}

void slowRight(){
  leftServo.writeMicroseconds(stopL+160);
  rightServo.writeMicroseconds(stopR+30);
}

void stop() {
  leftServo.writeMicroseconds(stopL);
  rightServo.writeMicroseconds(stopR);
}

void turnAround(){
  turnRight90();
  turnRight90();
}

void turnLeft90(){
  leftServo.writeMicroseconds(stopL-100);
  rightServo.writeMicroseconds(stopR+100);
  delay(1500);
  stop();
}

void slowLeft() {
  leftServo.writeMicroseconds(stopL+30);
  rightServo.writeMicroseconds(stopR+180);
}

void normRight() {
  leftServo.writeMicroseconds(stopL+300);
  rightServo.writeMicroseconds(stopR);
}

void normLeft() {
  leftServo.writeMicroseconds(stopL);
  rightServo.writeMicroseconds(stopR+300);
}


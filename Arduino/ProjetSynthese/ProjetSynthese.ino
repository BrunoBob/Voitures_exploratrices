/* Code test, fais avancer le robot, détécter les intersections, tourner
   Version 0.2
   Auteur : Bruno TESSIER & Paul VALENTIN
*/
#include <Servo.h>
#include <RH_ASK.h>
#include <RHReliableDatagram.h>

RH_ASK driverRF(2000, 9, 8, 10, false);
RHReliableDatagram RF(driverRF, 1);

/*Tolerance for Compass*/
#define TOLERANCE 2

/*
   Space tolerance for ultrasonic sensor, if space between robot and wall
   is lessier than SPTOL, robot move off the wall
*/
#define SPTOL 100

/*Constant for US sensor*/
const byte CENTER_T = 7 ;
const byte CENTER_E = 6 ;

const byte LEFT_T = 3 ;
const byte LEFT_E = 2 ;

const byte RIGHT_T = 5 ;
const byte RIGHT_E = 4 ;

const long MEASURE_TIMEOUT = 25000L;


/*ServoMotor*/
Servo servoRight;
Servo servoLeft;



const byte led = 10;

float myTime;


byte* message;


/*
   When the led is HIGH, you can turn the robot manually to another angle
*/
void setup() {
  Serial.begin(9600);

  RF.init();
  RF.setTimeout(300);
  RF.setRetries(5);
  message = (byte*)malloc(2);

  pinMode(CENTER_T, OUTPUT);
  pinMode(CENTER_E, INPUT);

  pinMode(LEFT_T, OUTPUT);
  pinMode(LEFT_E, INPUT);

  pinMode(RIGHT_T, OUTPUT);
  pinMode(RIGHT_E, INPUT);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  delay(10);
 // Serial.println("Fin du setup");
  servoRight.attach(13);
  servoLeft.attach(12);
}

/*
   Right wheel have to go to an angle of 124 and left wheel of 65 for go straihgt ahead
   95 is the angle where robot doesn't move

   Led is HIGH when robot go forward
*/
  
void loop() {
  /*The distance between robot and obstacle*/
  double mmCenter = 0;
  double mmLeft = 0;
  double mmRight = 0;
  double mmFreeSpace;
  

  /*When he can go forward*/
  /*
     TODO
     Check left and right side if we detect intersection
  */
  byte rightSpeed = 95;
  byte leftSpeed = 95;

    digitalWrite(led, HIGH);
    delay(10);
  /*While he doesn't detect any intersection*/
  myTime = millis();
  while ((mmCenter >= 50 || mmCenter == 0) && mmRight <= 400 && mmLeft <= 400) {
    /*if the robot can go faster*/
    if (rightSpeed < 110)
      rightSpeed += 2;
    if (leftSpeed > 80)
      leftSpeed -= 2;
    /*Check if robot is not too close of a wall*/
    if (((mmRight >= mmFreeSpace - SPTOL) && leftSpeed <= 95 && rightSpeed <= 106) && mmRight != 0 && mmLeft != 0) {
      //Serial.println("Robot go to the left");
      digitalWrite(led, LOW);
      leftSpeed += 2;
      rightSpeed += 2;
    }
    if (((mmLeft >= mmFreeSpace - SPTOL) && rightSpeed >= 95 && leftSpeed >= 84) && mmRight != 0 && mmLeft != 0){
      //Serial.println("Robot go to the right");
      digitalWrite(led, LOW);
      delay(10);
      digitalWrite(led, LOW);
      leftSpeed -= 2;
      rightSpeed -= 2;
    }
    digitalWrite(led, HIGH);

    servoLeft.write(leftSpeed);
    servoRight.write(rightSpeed);
    Serial.println("[SPEED]");
    Serial.print("leftSpeed = "); Serial.println(leftSpeed);
    Serial.print("rightSpeed = "); Serial.println(rightSpeed);
    Serial.println("[/SPEED]");
    delay(500);
    mmCenter = getDistance(0);
    mmLeft = getDistance(1);
    mmRight = getDistance(2);

    /*If one of two measure is 0, certainly a bug, so we don't update*/
    if (mmRight == 0 || mmLeft == 0) {}
    else {
      mmFreeSpace = mmLeft + mmRight;
    }
    Serial.println("[MEASURE]");
    Serial.print("mmLeft = "); Serial.println(mmLeft);
    Serial.print("mmRight = "); Serial.println(mmRight);
    Serial.print("mmCenter = "); Serial.println(mmCenter);
    Serial.print("Free Space = "); Serial.println(mmFreeSpace);
    Serial.println("[/MEASURE]\n");
  }
  myTime = millis() - myTime ;
  digitalWrite(led, LOW);
  delay(10);
  Serial.println("[STOP] Robot stop to go forward [/STOP]");
  servoLeft.write(95);
  servoRight.write(95);
  /*
     TODO
     Check Graph for exploration mode
  */
  /*
   * Envoi des données, 0=arrière 1=droite, 2=devant, 3=gauche
   */
   *message = 64;
  if(mmLeft >= 400 && mmRight >=400 &&mmCenter >= 400){
    /*
     * Nouveau noeud 4chemins 0 1 2 3
     */
     *(message+1) = 1;
  }
  else if(mmLeft >= 400 && mmRight >=400 &&mmCenter <= 400){
    
    /*
     * 0 1 2
     */
     *(message+1) = 6;
  }
  else if(mmLeft <= 400 && mmRight >=400 &&mmCenter >= 400){
    /*
     * 0 2 3
     */
     *(message+1) = 3;
  }
  else if(mmLeft >= 400 && mmRight <=400 &&mmCenter <= 400){
    /*
     * 0 1 
     */
     *(message+1) = 4;
  }
  else if(mmLeft <= 400 && mmRight <=400 &&mmCenter <= 400){
    /*
     * rien du tout 0000
     */
     *(message+1) = 7;
  }
  else if(mmLeft >= 400 && mmRight <=400 &&mmCenter >= 400){
    /*
     * 0 1 3
     */
     *(message+1) = 2;
  }
  else if(mmLeft <= 400 && mmRight >=400 &&mmCenter <= 400){
    /*
     * 0 1 3
     */
     *(message+1) = 5;
  }
  sendRF();
  delay(3000);  
}

void sendRF(){
  RF.sendtoWait(message, 2, 0);
}

/*
   Function that redirectto readSensor
   See to remove it
*/
double getDistance(byte sensor) { //0 = center  1=left  2=right
  if (sensor == 0) {
    return (readSensor(CENTER_T, CENTER_E));
  }
  else if (sensor == 1) {
    return (readSensor(LEFT_T, LEFT_E));
  }
  else if (sensor == 2) {
    return (readSensor(RIGHT_T, RIGHT_E));
  }
}

/*
   Function permit hat read ultrasonic sensors and return the distance
*/
double readSensor(byte trigger, byte echo) {
  digitalWrite(trigger, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigger, LOW);

  double distance = (double) pulseIn(echo, HIGH, MEASURE_TIMEOUT);
  distance = distance  / 2 * 0.34 ;

  if (!distance) { //If bug long distance then reset the sensor
    pinMode(echo, OUTPUT);
    digitalWrite(echo, LOW);
    pinMode(echo, INPUT);
  }
  return distance ;
}

/*Function permit to turn the robot to a certain angle*/

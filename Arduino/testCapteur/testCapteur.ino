/*Code pour faire avancer un robot
 * Version 0
 * Auteur : Bruno TESSIER Paul VALENTIN
 */
#include <Servo.h>

const byte CENTER_T = 7 ;
const byte CENTER_E = 6 ;

const byte LEFT_T = 3 ;
const byte LEFT_E = 2 ;

const byte RIGHT_T = 5 ;
const byte RIGHT_E = 4 ;

Servo servoRight;
Servo servoLeft;

const long MEASURE_TIMEOUT = 25000L;

void setup(){
  Serial.begin(9600);
  
  servoRight.attach(13);
  servoLeft.attach(12);
  pinMode(CENTER_T, OUTPUT);
  pinMode(CENTER_E, INPUT);
  
  pinMode(LEFT_T, OUTPUT);
  pinMode(LEFT_E, INPUT);
  
  pinMode(RIGHT_T, OUTPUT);
  pinMode(RIGHT_E, INPUT);
}

void loop(){
  double mmCenter;
  double mmLeft;
  double mmRight;
  
 mmCenter = getDistance(0);
 Serial.print("center : ");
 Serial.print(mmCenter);
 
 mmLeft = getDistance(1);
 Serial.print("left : ");
 Serial.print(mmLeft);
 
 mmRight = getDistance(2);
 Serial.print("right : ");
 Serial.println(mmRight);
 
 if(mmCenter >= 50 && mmCenter != 0){
   servoLeft.write(65);
   servoRight.write(124);
 }else{
   servoLeft.write(94);
   servoRight.write(95);
 }
 
 delay(120);
}

double getDistance(byte sensor){ //0 = center  1=left  2=right
  if(sensor==0){
    return (readSensor(CENTER_T, CENTER_E));
  }
  else if(sensor==1){
    return (readSensor(LEFT_T, LEFT_E));
  }
  else if(sensor==2){
    return (readSensor(RIGHT_T, RIGHT_E));
  }
}

double readSensor(byte trigger, byte echo){
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  double distance = (double) pulseIn(echo, HIGH, MEASURE_TIMEOUT);
  distance = distance  / 2 * 0.34 ;
  
  if(!distance){ //If bug long distance then reset the sensor
    pinMode(echo, OUTPUT);
    digitalWrite(echo, LOW);
    pinMode(echo, INPUT);
  }
  return distance ;
}  

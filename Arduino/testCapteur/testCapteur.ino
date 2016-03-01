/*Code pour faire avancer un robot
 * Version 0
 * Auteur : Bruno TESSIER Paul VALENTIN
 */
#include <Servo.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>

/*Tolerance for Compass*/
#define TOLERANCE 10

/*
 * Space tolerance for ultrasonic sensor, if space between robot and wall
 * is lessier than SPTOL, robot move off the wall
 */
#define SPTOL 20

HMC5883L_Simple Compass;

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

/*
 * Contains value of each 90° angle
 * Permit to test if robot do an 90° rotation
 * 
 */
float degres[4]; 
byte currentAngle = 0; //First angle is set to 0

const byte led = 8;
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

  pinMode(led, OUTPUT);

  Wire.begin();

  Compass.SetDeclination(0, 14, 'E');  
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_560);
  Compass.SetOrientation(COMPASS_HORIZONTAL_Y_NORTH);

  /*Setup for the 90 degres*/
  degres[0] = Compass.GetHeadingDegrees();
  Serial.print("Angle [0] = ");
  Serial.println(degres[0]);
  int i = 0;
  for(i=1;i<=3;i++){
    digitalWrite(led,HIGH);
    servoLeft.write(93);
    servoRight.write(93);
    delay(4000); //bad delay, better to setup manually
    digitalWrite(led,LOW);   
    servoLeft.write(95);
    servoRight.write(95);
    degres[i] = Compass.GetHeadingDegrees();
    Serial.print("Angle [");Serial.print(i);Serial.print("] = ");
    Serial.println(degres[i]);
    delay(1000);
  }
 Serial.println("Fin du setup");
 delay(1000);
}

/*
 * Right wheel have to go to an angle of 124 and left wheel of 65 for go straihgt ahead
 * 95 is the angle where robot doesn't move
 */
void loop(){
  /*The distance between robot and obstacle*/
 double mmCenter=0;
 double mmLeft=0;
 double mmRight=0;
 double mmFreeSpace;
 
 float currentDegre;
 currentDegre = 0;
 
 mmCenter = getDistance(0);
 mmLeft = getDistance(1);
 mmRight = getDistance(2);
 mmFreeSpace = mmLeft + mmRight;

 /*When he can go forward*/
 /*TODO
  * Check left and right side if we detect intersection
  */
 while((mmCenter >= 50 || mmCenter == 0)){
  digitalWrite(led,HIGH);
  byte rightSpeed = 95;
  byte leftSpeed = 95;
  /*if the robot can go faster*/
  if(rightSpeed <= 124)
    rightSpeed += 2;
  if(leftSpeed >= 65)
    leftSpeed -= 2;
  /*Check if robot is not too close to the wall*/  
  if(mmFreeSpace - mmLeft >= mmFreeSpace - SPTOL && leftSpeed <= 95){
    Serial.println("Robot go to the right");
    leftSpeed--;
    rightSpeed++;
  }
  if(mmFreeSpace - mmRight >= mmFreeSpace - SPTOL && rightSpeed >= 95){
    Serial.println("Robot go to the left");
    rightSpeed--;
    leftSpeed++;
    Serial.println("Robot go to the right");
  }
  
   servoLeft.write(leftSpeed);
   servoRight.write(rightSpeed);
   Serial.print("leftSpeed = ");Serial.println(leftSpeed);
   Serial.print("rightSpeed = ");Serial.println(rightSpeed);
   delay(500);
   mmCenter = getDistance(0);
   mmLeft = getDistance(1);
   mmRight = getDistance(2);
   if(mmRight == 0 || mmLeft == 0){}
   else{
    mmFreeSpace = mmLeft + mmRight;
   }
   Serial.print("mmLeft = ");Serial.println(mmLeft);
   Serial.print("mmRight = ");Serial.println(mmRight);
   Serial.print("mmCenter = ");Serial.println(mmCenter);
   Serial.print("Free Space = ");Serial.println(mmFreeSpace);
 }
 digitalWrite(led,LOW);
 Serial.println("STOP Robot stop to go forward STOP");
 servoLeft.write(95);
 servoRight.write(95);
 /*
  * TODO
  * Check Graph for exploration mode
  */
 if(mmRight >= 100){
  Serial.println("Robot begin to rotate counterclockewise");
  /*if we go at our right, and during the setup robot rotate counterclockerwise */
  if(currentAngle < 3){
    currentAngle++;
  }else{
    currentAngle = 0;
  }
    
  while((currentDegre <= (degres[currentAngle] - TOLERANCE)) || (currentDegre >= (degres[currentAngle] + TOLERANCE))){
      currentDegre = Compass.GetHeadingDegrees();
      servoLeft.write(93);
      servoRight.write(93);
      delay(300);
      servoLeft.write(95);
      servoRight.write(95);
      delay(100);
  }
   servoLeft.write(93);
   servoRight.write(93);
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
  delayMicroseconds(50);
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

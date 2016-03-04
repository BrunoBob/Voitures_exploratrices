/* Code test, fais avancer le robot, détécter les intersections, tourner
   Version 0.2
   Auteur : Bruno TESSIER & Paul VALENTIN
*/
#include <Servo.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>

/*Tolerance for Compass*/
#define TOLERANCE 10

/*
   Space tolerance for ultrasonic sensor, if space between robot and wall
   is lessier than SPTOL, robot move off the wall
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
   Contains value of each 90° angle
   Permit to test if robot do an 90° rotation

*/
float degres[4];
byte currentAngle = 0; //First angle is set to 0
float currentDegre;

const byte led = 8;

/*
   When the led is HIGH, you can turn the robot manually to another angle
*/
void setup() {
  Serial.begin(9600);

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
  for (uint8_t i = 1; i <= 3; i++) {
    digitalWrite(led, HIGH);

    delay(4000); //bad delay, better setup manually
    digitalWrite(led, LOW);

    degres[i] = Compass.GetHeadingDegrees();
    Serial.print("Angle ["); Serial.print(i); Serial.print("] = ");
    Serial.println(degres[i]);
    delay(1000);
  }
 // Serial.println("Fin du setup");
  servoRight.attach(13);
  servoLeft.attach(12);
  delay(3000);
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

  currentDegre = 0;

  mmCenter = getDistance(0);
  mmLeft = getDistance(1);
  mmRight = getDistance(2);
  mmFreeSpace = mmLeft + mmRight;

  /*When he can go forward*/
  /*
     TODO
     Check left and right side if we detect intersection
  */
  byte rightSpeed = 95;
  byte leftSpeed = 95;

    digitalWrite(led, HIGH);
  /*While he doesn't detect any intersection*/
  while ((mmCenter >= 50 || mmCenter == 0) && mmRight <= 400 && mmLeft <= 400) {
    /*if the robot can go faster*/
    if (rightSpeed <= 124)
      rightSpeed += 2;
    if (leftSpeed >= 65)
      leftSpeed -= 2;
    /*Check if robot is not too close of a wall*/
    if (mmFreeSpace - mmLeft >= mmFreeSpace - SPTOL && leftSpeed <= 95) {
      //Serial.println("Robot go to the left");
      leftSpeed--;
      rightSpeed++;
    }
    if (mmFreeSpace - mmRight >= mmFreeSpace - SPTOL && rightSpeed >= 95) {
      //Serial.println("Robot go to the right");
      rightSpeed--;
      leftSpeed++;
    }

    servoLeft.write(leftSpeed);
    servoRight.write(rightSpeed);
    /*Serial.println("[SPEED]");
    Serial.print("leftSpeed = "); Serial.println(leftSpeed);
    Serial.print("rightSpeed = "); Serial.println(rightSpeed);
    Serial.println("[/SPEED]");*/
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
  digitalWrite(led, LOW);
  //Serial.println("[STOP] Robot stop to go forward [/STOP]");
  servoLeft.write(95);
  servoRight.write(95);
  /*
     TODO
     Check Graph for exploration mode
  */
  if (mmRight >= 400) {
    Serial.println("[ROTATE]Robot begin to rotate counterclockewise[/ROTATE]");
    /*if we go at our left, and during the setup robot rotate counterclockerwise */
    if (currentAngle > 0) {
      currentAngle--;
    } else {
      currentAngle = 3;
    }
    turn();
  }
  else if (mmLeft >= 400) {
    Serial.println("[ROTATE]Robot begin to rotate clockewise[/ROTATE]");
    /*if we go at our right, and during the setup robot rotate counterclockerwise */
    if (currentAngle < 3) {
      currentAngle++;
    } else {
      currentAngle = 0;
    }
    turn();
  }
  else {
    Serial.println("[ROTATE]Robot begin to turn back[/ROTATE]");
    if (currentAngle < 2) {
      currentAngle += 2;
    } else {
      currentAngle -= 2;
    }
    turn();
  }
/*
  mmLeft = 0;
  mmCenter = 4000;
  mmRight = 0;
*/
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
void turn() {
  while ((currentDegre <= (degres[currentAngle] - TOLERANCE)) || (currentDegre >= (degres[currentAngle] + TOLERANCE))) {
    Serial.print("\ncurrent degre : ");Serial.println(currentDegre);
    Serial.print("goal : ");Serial.println(degres[currentAngle]);
    currentDegre = Compass.GetHeadingDegrees();
    servoLeft.write(93);
    servoRight.write(93);
    delay(300);
    servoLeft.write(95);
    servoRight.write(95);
    delay(100);
  }
}

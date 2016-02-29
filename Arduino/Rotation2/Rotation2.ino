  #include <Servo.h>
#include <Wire.h> //I2C Arduino Library
#include <HMC5883L_Simple.h>

#define TOLERANCE 10

Servo servoRight;
Servo servoLeft;

HMC5883L_Simple Compass;
  
/*
 * Contains value of each 90° angle
 * Permit to test if robot do an 90° rotation
 * 
 */
float degres[4]; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servoRight.attach(13);
  servoLeft.attach(12);

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
    servoLeft.write(93);
    servoRight.write(93);
    delay(5000);   
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

//-----------------------------------------------
//-----------------------------------------------

void loop() {
    float currentDegre;
    currentDegre = 0;
    delay(200);
     while((currentDegre <= (degres[2] - TOLERANCE)) || (currentDegre >= (degres[2] + TOLERANCE))){
      currentDegre = Compass.GetHeadingDegrees();
      servoLeft.write(93);
      servoRight.write(93);
      delay(300);
      servoLeft.write(95);
      servoRight.write(95);
      delay(100);
      
      Serial.print("Valeur de l'angle sur Y : ");
      Serial.println(currentDegre);
      Serial.print("Angle [2] = ");
      Serial.println(degres[2]);
      Serial.print("(currentDegre <= (degres[2] - TOLERANCE)) || (currentDegre >= (degres[2] + TOLERANCE)) = ");
      Serial.println((currentDegre <= (degres[1] - TOLERANCE)) || (currentDegre >= (degres[1] + TOLERANCE)));
    }
 
     Serial.print("Fin d'une boucle :\n");
     while(1){
      }
}

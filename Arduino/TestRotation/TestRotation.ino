#include <Servo.h>
#include <Wire.h> //I2C Arduino Library

#define TOLERANCE 20

Servo servoRight;
Servo servoLeft;


#define HMC_address  0x1E

/* 
 *  Union, each data have 2*8bits
 *  We need to swap because of Endianess
 *  
 */
union Compass{
  struct{
    int8_t x_h;
    int8_t x_l;
    
    int8_t z_h;
    int8_t z_l;
    
    int8_t y_h;
    int8_t y_l;
  }reg;
  struct{
    int16_t x;
    int16_t z;
    int16_t y;
  }value;
};

/*
 * Contains value of each 90° angle
 * Permit to test if robot do an 90° rotation
 * 
 */
Compass degres[4]; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servoRight.attach(13);
  servoLeft.attach(12);
  
  Wire.beginTransmission(HMC_address); //start talking
  Wire.write(0x02); // Set the Register
  Wire.write(0x00); // Tell the HMC5883 to Continuously Measure
  Wire.endTransmission();

  /*Setup for the 90 degres*/
  degres[0] = getCompass();
  Serial.print("Angle [0] = ");
  Serial.println(degres[0].value.z);
  int i = 0;
  for(i=1;i<=3;i++){
    servoLeft.write(65);
    servoRight.write(65);
    delay(1000);   
    servoLeft.write(95);
    servoRight.write(95);
    degres[i] = getCompass();
    Serial.print("Angle [");Serial.print(i);Serial.print("] = ");
    Serial.println(degres[i].value.z);
    delay(1000);
  }
 Serial.println("Fin du setup");
 delay(1000);
}

//-----------------------------------------------
//-----------------------------------------------

void loop() {
    Compass currentDegre;
    currentDegre.value.z = 0;
    delay(200);
     while((currentDegre.value.z <= (degres[2].value.z - TOLERANCE)) || (currentDegre.value.z >= (degres[2].value.z + TOLERANCE))){
      currentDegre = getCompass();
      servoLeft.write(65);
      servoRight.write(65);
      delay(300);
      servoLeft.write(95);
      servoRight.write(95);
      delay(100);
      
      Serial.print("Valeur de l'angle sur Z : ");
      Serial.println(currentDegre.value.z);
      Serial.print("Angle [2] = ");
      Serial.println(degres[2].value.z);
      Serial.print("(currentDegre.value.z <= (degres[2].value.z - TOLERANCE)) || (currentDegre.value.z >= (degres[2].value.z + TOLERANCE)) = ");
      Serial.println((currentDegre.value.z <= (degres[2].value.z - TOLERANCE)) || (currentDegre.value.z >= (degres[2].value.z + TOLERANCE)));
    }
 
     Serial.print("Fin d'une boucle :\n");
     while(1){
        Serial.print("J'ai fini");
      }
}

//-----------------------------------------------
//-----------------------------------------------

/*
 * get the value of Compass
 */
Compass getCompass(){
  Compass compass;
   int error;

  error = HMC_read(0x03, (uint8_t *) &compass, sizeof(compass));

  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (compass.reg.x_h, compass.reg.x_l);
  SWAP (compass.reg.y_h, compass.reg.y_l);
  SWAP (compass.reg.z_h, compass.reg.z_l);
  
  return compass;
}

/*
 * read the HMC interface
 */

int HMC_read(int start, uint8_t *buffer, int size){
  int i, n;
    //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC_address);
  n = Wire.write(start); //select register 3, X MSB register
  if(n != 1)
    return -10;
  n = Wire.endTransmission(false);
  if (n)
    return n;
    
  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC_address, size, true);
  i = 0;
  while(Wire.available() && i < size){
    buffer[i++] = Wire.read();
  }
  if(i != size)
    return -11;

  return 0;
}

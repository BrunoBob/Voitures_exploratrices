/*
 * Programe by Paul VALENTIN
 * 
 * Read the compass
 */
#include <Wire.h>

#define HMC_address  0x1E

/* Union, each data have 2*8bits
 *  We need to swap because of Endianess
 */
typedef union compass{
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
void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(HMC_address);
  Wire.write(0x02); //Continuous measure mode
  Wire.write(0x00); 
  Wire.endTransmission();
}

void loop() {
  compass compass;
  int error;

  error = HMC_read(0x03, (uint8_t *) &compass, sizeof(compass));

  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (compass.reg.x_h, compass.reg.x_l);
  SWAP (compass.reg.y_h, compass.reg.y_l);
  SWAP (compass.reg.z_h, compass.reg.z_l);
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(compass.value.x);
  Serial.print("  y: ");
  Serial.print(compass.value.y);
  Serial.print("  z: ");
  Serial.println(compass.value.z);
  
  delay(250);
}

/*******************************
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


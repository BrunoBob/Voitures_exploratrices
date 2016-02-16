#include <Servo.h>

const byte TRIGGER_RIGHT = 3; //Broche Trigger
const byte ECHO_RIGHT = 2; //Broche ECHO

const byte TRIGGER_LEFT = 5;
const byte ECHO_LEFT = 4;

const byte TRIGGER_CENTER = 7;
const byte ECHO_CENTER = 6;

Servo servoRight;
Servo servoLeft;

const long TIMEOUT = 25000L; //40Hz

void setup(){
  
  Serial.begin(9600);
  
  servoRight.attach(13);
  servoLeft.attach(12);
  
  pinMode(TRIGGER_RIGHT,OUTPUT);
  pinMode(TRIGGER_LEFT,OUTPUT);
  pinMode(TRIGGER_CENTER,OUTPUT);
  
  pinMode(ECHO_RIGHT,INPUT);
  pinMode(ECHO_LEFT,INPUT);
  pinMode(ECHO_CENTER,INPUT);
  
  digitalWrite(TRIGGER_RIGHT, LOW);
  digitalWrite(TRIGGER_LEFT, LOW);
  digitalWrite(TRIGGER_CENTER, LOW);
}

void loop(){
  
  long measure;
  long mmRight;
  long mmLeft;
  long mmCenter;
  
  digitalWrite(TRIGGER_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_RIGHT, LOW);
  
  measure = pulseIn(ECHO_RIGHT, HIGH, TIMEOUT);
  
  if(!measure){
    pinMode(ECHO_RIGHT,OUTPUT);
    digitalWrite(ECHO_RIGHT,LOW);
    delay(10);
    //digitalWrite(ECHO_PIN,HIGH);
    pinMode(ECHO_RIGHT, INPUT);
  }
  
  mmRight = measure / 2 * 0.34;

Serial.println("Right :");
Serial.print(F("Distance: "));
  Serial.print(mmRight);
  Serial.print(F("mm ("));
  Serial.print(mmRight / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(mmRight / 1000.0, 2);
  Serial.println(F("m)"));
   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  //delay(120);

/*************************************************/

  digitalWrite(TRIGGER_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_LEFT, LOW);
  
   measure = pulseIn(ECHO_LEFT, HIGH, TIMEOUT);
  
  if(!measure){
    pinMode(ECHO_LEFT,OUTPUT);
    digitalWrite(ECHO_LEFT,LOW);
    delay(10);
    //digitalWrite(ECHO_PIN,HIGH);
    pinMode(ECHO_LEFT, INPUT);
  }
  
   mmLeft = measure / 2 * 0.34;
   
  Serial.println("Left : ");
  Serial.print(F("Distance: "));
  Serial.print(mmLeft);
  Serial.print(F("mm ("));
  Serial.print(mmLeft / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(mmLeft / 1000.0, 2);
  Serial.println(F("m)"));
   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  //delay(120);
/****************************************************/
digitalWrite(TRIGGER_CENTER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_CENTER, LOW);
  
   measure = pulseIn(ECHO_CENTER, HIGH, TIMEOUT);
  
  if(!measure){
    pinMode(ECHO_CENTER,OUTPUT);
    digitalWrite(ECHO_CENTER,LOW);
    delay(10);
    //digitalWrite(ECHO_PIN,HIGH);
    pinMode(ECHO_CENTER, INPUT);
  }
  
   mmCenter = measure / 2 * 0.34;
  
    /* Affiche les résultats en mm, cm et m */
   Serial.println("Center");
  Serial.print(F("Distance: "));
  Serial.print(mmCenter);
  Serial.print(F("mm ("));
  Serial.print(mmCenter / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(mmCenter / 1000.0, 2);
  Serial.println(F("m)"));
/*Partie ServoMoteur*/
  if(mmCenter >= 50 && mmCenter != 0){
    servoLeft.write(65);
    servoRight.write(124);
    delay(10);
  }else{
   servoLeft.write(94);
   servoRight.write(95);
   delay(10);
  }   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(120);
    
}

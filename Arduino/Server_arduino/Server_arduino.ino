#include <RH_ASK.h>
#include <RHReliableDatagram.h>
/*#include <ThreadController.h>
#include <Thread.h>
#include <TimerOne.h>
#include <Servo.h>
#include <Wire.h>*/

RH_ASK driverRF(2000, 9, 8, 10, false);
RHReliableDatagram RF(driverRF, 0);

byte* message;

void setup() {
  RF.init();
  Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
  message = (byte*)malloc(4);
}

void loop() {
  if(RF.available()){
    readRF();
  }
  /*if(Serial.available()>5){
    readSerial();
  }*/
}

void readRF(){
  uint8_t buflen = 4;
  if(RF.recvfromAck(message, &buflen, NULL, NULL, NULL, NULL)){
      writeSerial();
  }
}

void sendRF(){
  RF.sendto(message, 4, *message);
  RF.waitPacketSent();
}

void readSerial(){
  byte buff = Serial.read();
  while(buff != '#'){
    buff = Serial.read();
  }
  for(int i=0; i<4 ;i++){
    *(message+i) = Serial.read();
  }
  buff = Serial.read();
  if(buff == '!'){
    sendRF();
  }
}

void writeSerial(){
  Serial.write('#');
  for(int i = 0 ; i < 4 ; i++){
    Serial.write(*(message+i));
  }
  Serial.write('!');
}


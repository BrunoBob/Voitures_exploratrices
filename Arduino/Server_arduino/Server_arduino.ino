#include <RH_ASK.h>

RH_ASK driverRF(2000, 9, 8, 10, false);

byte* message;

void setup() {
    driverRF.init();
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    message = (byte*)malloc(4);
    *(message) = 1;
  *(message+1) = 0;
  *(message+2) = 1;
  *(message+3) = 0;
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
}

void loop() {
  if(driverRF.available()){
    readRF();
  }
  if(Serial.available()>5){
    readSerial();
  }
}

void readRF(){
  uint8_t buflen = 4;
  if(driverRF.recv(message, &buflen)){
    if(message[0] == 0){
      writeSerial();  
    }
  }
}

void sendRF(){
       digitalWrite(7,LOW);
    delay(50);
      digitalWrite(7,HIGH);

  driverRF.send(message, 4);
  driverRF.waitPacketSent();
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
 
    sendRF();//sendRF();sendRF();sendRF();sendRF();sendRF();
  }
}

void writeSerial(){
  Serial.write('#');
  for(int i = 0 ; i < 4 ; i++){
    Serial.write(*(message+i));
  }
  Serial.write('!');
}


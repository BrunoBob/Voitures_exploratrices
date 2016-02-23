#include <Servo.h>
#include <SPI.h>
#include <RH_ASK.h>
 
const uint8_t rxPin = 9;

RH_ASK driverRF(2000, rxPin, 8, 10, false);
 
void setup() // Fonction setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Tuto VirtualWire"); // Petit message de bienvenue
    if(!driverRF.init())
      Serial.println("init failed");
    
}
 
void loop() // Fonction loop()
{ 
    if (driverRF.available()) // Si un message est reçu dans les 200ms qui viennent
    {
      readMsg();
    }
}

void readMsg(){
  uint8_t buf[3]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
  uint8_t buflen = 3; // Taille maximum de notre tableau
    
  if (driverRF.recv(buf, &buflen)){  // On copie le message, qu'il soit corrompu ou non
    Serial.print("RX : ");
    if(buf[0] == 80){
      Serial.print(" P ");
      
      // move tilt to buf[1]
      Serial.print(" - tilt = ");
      Serial.print(buf[1]);
      
      // move pan to buf[2]
      Serial.print(" - pan = ");
      Serial.print(buf[2]);
      
    }
    Serial.println("");
    Serial.println("ok");
  }
}

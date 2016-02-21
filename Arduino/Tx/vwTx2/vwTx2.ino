#include <Servo.h>
#include <SPI.h>
#include <RH_ASK.h>
 
const uint8_t txPin = 8;

RH_ASK driverRF(2000, 9, txPin, 10, false);
  char *msg = "   ";

 
void setup() // Fonction setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("RF setup"); // Petit message de bienvenue
    
    if(!driverRF.init())
      Serial.println("init failed");
}
 
void loop() // Fonction loop()
{
    msg[0] = 'P' ; //caractére P de début de message

    Serial.print("TX ... "); // On signale le début de l'envoi
    sendMsg(20,30);
    Serial.println("Done !"); // On signal la fin de l'envoi
    driverRF.init();
    
}

void sendMsg(char tilt, char pan){
  msg[1] = tilt, 
  msg[2] = pan;
  driverRF.send((uint8_t *) msg,strlen(msg)); //Envoi du message
  driverRF.waitPacketSent();
}



#include <Servo.h>
#include "ServoControl.h"

// Définition de la broche pour le servo
#define PinServoContinuous 11

// Création d'un objet Servo
Servo servoContinuous;

void setup() 

{
    // Initialisation du servo
    vInitializeServo(servoContinuous, PinServoContinuous);
    Serial.begin(9600);
}

void loop() 
{
  Serial.println("Ajustement de la hauteur à 10 mm...");
  vAdjustHeight(servoContinuous, 10);
  delay(2000);
  Serial.println("Ajustement de la hauteur à -10 mm...");
  vAdjustHeight(servoContinuous, -10);
  delay(2000);
}

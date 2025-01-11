#include <Arduino.h>
#include "RobotPutterSensors.h"

UltrasonicSensors Usensors(13, 2 , 13, 4); // Pins for sensor 1 and 2

void setup() 
{
    Serial.begin(9600);
}

void loop() 
{
    sSensorValues sDistances = Usensors.sReadDistances();
    Serial.print("Distance Droite: ");
    Serial.print(sDistances.fDistance1);
    Serial.print("cm, Distance Arrière: ");
    Serial.print(sDistances.fDistance2);
    Serial.println(" cm");
    delay(1000);
}
#include <Arduino.h>
#include "ServoControl.h"

Servo myServo;

void setup() 
{
    vInitializeServo(myServo, 3);  // Attach servo to pin 9
}

void loop() 
{
    vRotateByAngle(myServo, 180);   // Rotate by 90° left
    delay(1000);
    vRotateByAngle(myServo, 90);
    delay(1000);
    vSlowSweep(myServo);           // Perform slow sweep
    delay(1000);
}

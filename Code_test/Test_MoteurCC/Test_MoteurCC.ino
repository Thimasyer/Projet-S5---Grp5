#include <Arduino.h>
#include <MotorControl.h>

// numéro de pin basé sur le schéma élec de notre PCB
#define DIR1 8
#define PWM1 9
#define PWM2 10
#define DIR2 7


void setup() 
{
    Serial.begin(9600);
    vMotorDriverInit(DIR1, PWM1, DIR2, PWM2);   // 1 = Left 2 = Right
}


void loop()
{
  // Test des moteurs
    vSetMotor(MOTOR_RIGHT, FORWARD, 250);
    vSetMotor(MOTOR_LEFT, FORWARD, 250);
    delay(1000);
    vSetMotor(MOTOR_RIGHT, BACKWARD, 250);
    vSetMotor(MOTOR_LEFT, BACKWARD, 250);
    delay(4000);
  
}
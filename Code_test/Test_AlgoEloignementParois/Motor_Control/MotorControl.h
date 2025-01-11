/***********  FILE HEADER ********************************************************
 * @file MotorControl.h
 * @brief Header for controlling two DC motors using a motor driver.
 *********************************************************************************/
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <stdint.h>

// Définition des moteurs
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1
// Définition des directions du moteur
typedef enum 
{
    FORWARD = 0,
    BACKWARD
} MotorDirection;

// Prototypes des fonctions
void vMotorDriverInit(uint16_t u16DirPinL, uint16_t u16PwmPinL, uint16_t u16DirPinR, uint16_t u16PwmPinR);
void vSetMotor(uint16_t u16MotorID, MotorDirection eDirection, uint16_t u16Speed);
void vStopMotor(uint16_t u16MotorID);

#endif // MOTORCONTROL_H

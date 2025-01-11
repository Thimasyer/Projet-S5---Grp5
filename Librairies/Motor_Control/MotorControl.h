/***********  FILE HEADER ********************************************************
 * @file MotorControl.h
 * @brief Header for controlling two DC motors using a motor driver.
 *********************************************************************************/
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

// -----------------------------------------------------------------------------
// Lib packages includes, compiler define
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <stdint.h>
// Définition des pins des moteurs CC 
#define PinDir1 7            // moteur droit
#define PinPWM1 9
#define PinPWM2 10
#define PinDir2 8            // moteur gauche
// Définition des moteurs
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// -----------------------------------------------------------------------------
// Declaration structs, unions, enums, bitfields, typedefs
// -----------------------------------------------------------------------------
// Définition des directions du moteur
typedef enum 
{
    FORWARD = 0,
    BACKWARD
} MotorDirection;

//------------------------------------------------------------------------------
// Declaration fonctions                                           
//------------------------------------------------------------------------------
// Prototypes des fonctions
void vMotorDriverInit(uint16_t u16DirPinL, uint16_t u16PwmPinL, uint16_t u16DirPinR, uint16_t u16PwmPinR);
void vSetMotor(uint16_t u16MotorID, MotorDirection eDirection, uint16_t u16Speed);
void vStopMotor(uint16_t u16MotorID);

#endif // MOTORCONTROL_H

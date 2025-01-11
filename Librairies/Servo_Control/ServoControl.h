/***********  COPMMENTAIRE DOXYGEN D'EN TETE-FICHIER ***************************
 * @file ServoStandard.h
 * @brief Library for controlling standard servo motor
 * This library provides functions to control the position standard
 * servo motor using PWM (Pulse Width Modulation).
 *****************************************************************************/
#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

// -----------------------------------------------------------------------------
// Lib packages includes, compiler define
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <stdint.h>
#include <Servo.h>
// Définition des pins des servos
#define PinServoU 3
#define PinServoRampe 5           
#define PinServoPlateforme 6
#define DEBUG 0                 // Set to 1 to enable debug messages
#define POS_U_HORRIZ 45         // Position horizontale du servo U, pour garder la ballle
#define POS_U_VERT 100          // Position verticale du servo U, pour attrpaer la balle

// -----------------------------------------------------------------------------
// Declaration structs, unions, enums, bitfields, typedefs
// -----------------------------------------------------------------------------
/**
 * @struct sServoPosition
 * @brief Structure to store the current position of the servo motor.
 */
struct sServoPosition 
{
    uint8_t u8CurrentAngle; /**< Current angle of the servo motor (in degrees) */
};

//------------------------------------------------------------------------------
// Declaration of functions                                           
//------------------------------------------------------------------------------
void vInitializeServo(Servo& servo, uint8_t cu8Pin);
void vRotateByAngle(Servo& servo, uint8_t u8Angle);
void vSlowSweep(Servo& servo);
void vAdjustHeight(Servo& servo, int16_t height);

#endif // SERVOSTANDARD_H

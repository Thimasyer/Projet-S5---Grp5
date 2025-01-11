/***********  FILE HEADER ********************************************************
 * @file MotorControl.cpp
 * @brief Implementation for controlling two DC motors using a motor driver.
 *********************************************************************************/
#include "MotorControl.h"
#include "Arduino.h"

// Variables globales pour les pins des moteurs
// static = variable accessible uniquement dans ce fichier
static uint16_t su16DirPinL, su16PwmPinL; // Pins pour le moteur gauche
static uint16_t su16DirPinR, su16PwmPinR; // Pins pour le moteur droit

/**
 * @brief Initialisation des pins du driver moteur
 * @param u16DirPinL Pin direction pour le moteur gauche
 * @param u16PwmPinL Pin PWM pour le moteur gauche
 * @param u16DirPinR Pin direction pour le moteur droit
 * @param u16PwmPinR Pin PWM pour le moteur droit
 */
void vMotorDriverInit(uint16_t u16DirPinL, uint16_t u16PwmPinL, 
                    uint16_t u16DirPinR, uint16_t u16PwmPinR)
{
    su16DirPinL = u16DirPinL;
    su16PwmPinL = u16PwmPinL;
    su16DirPinR = u16DirPinR;
    su16PwmPinR = u16PwmPinR;

    pinMode(su16DirPinL, OUTPUT);
    pinMode(su16PwmPinL, OUTPUT);
    pinMode(su16DirPinR, OUTPUT);
    pinMode(su16PwmPinR, OUTPUT);
}

/**
 * @brief Contrôle un moteur (vitesse et direction)
 * @param u16MotorID Identifiant du moteur (MOTOR_LEFT ou MOTOR_RIGHT)
 * @param eDirection Direction du moteur (FORWARD = LOW ou BACKWARD = HIGH)
 * @param u16Speed Vitesse du moteur (0 à 255)
 */
void vSetMotor(uint16_t u16MotorID, MotorDirection eDirection, uint16_t u16Speed)
{
 uint16_t u16DirPin;
 uint16_t u16PwmPin;

    // Déterminer les pins en fonction du moteur
    if (u16MotorID == MOTOR_LEFT)
    {
        u16DirPin = su16DirPinL;
        u16PwmPin = su16PwmPinL;
    }
    else if (u16MotorID == MOTOR_RIGHT)
    {
        u16DirPin = su16DirPinR;
        u16PwmPin = su16PwmPinR;
    }
    else
    {
        #if DEBUG
        Serial.println("Invalid motor ID");
        #endif
        return;
    }

    // Configurer la direction
    if (eDirection == FORWARD)
    {
        digitalWrite(u16DirPin, LOW);
    }
    else // BACKWARD
    {
        digitalWrite(u16DirPin, HIGH);
    }
    // Configurer la vitesse
    analogWrite(u16PwmPin, u16Speed);
}


/**
 * @brief Arrête un moteur
 * @param u16MotorID Identifiant du moteur à arrêter
 */
void vStopMotor(uint16_t u16MotorID)
{
 uint16_t u16PwmPin;
    // Déterminer le pin PWM en fonction du moteur
    if (u16MotorID == MOTOR_LEFT)
    {
        u16PwmPin = su16PwmPinL;
    }
    else if (u16MotorID == MOTOR_RIGHT)
    {
        u16PwmPin = su16PwmPinR;
    }
    else
    {
        #if DEBUG
        Serial.println("Invalid motor ID");
        #endif
        return;
    }
    // Mettre la vitesse à 0
    analogWrite(u16PwmPin, 0);
}

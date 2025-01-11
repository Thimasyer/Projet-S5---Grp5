/***********  COPMMENTAIRE DOXYGEN D'EN TETE-FICHIER ***************************
 * @file RobotPutterSensors.h
 * @brief Librairie pour lire les valeurs des différents capteurs.
 * This library provides:
 *      - a function to read the distance measured by
 *        two HC-SR04 ultrasonic sensors, returning the values as a structure.
 *      - a fonction to read the values of the line sensors, 
 *        returning the values as a structure.
 * 
 ******************************************************************************/
#ifndef ROBOTPUTTERSENSORS_H
#define ROBOTPUTTERSENSORS_H

// -----------------------------------------------------------------------------
// Lib packages includes, compiler define
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <stdint.h>
// Définition des pins des capteurs ultrasons
#define PinUsTrig 13
#define PinUsEchoDroit 2      // Droite
#define PinUsEchoArriere 4    // Arrière
// Définition des pins capteur IR
#define PinIR1 A2       // broche 1 du QRD114, celle relié à 10kOhm puis 5V
#define PinIR2 A3
#define PinIR3 A4
#define LINE_TRESHOLD 3.0 // Seuil pour considérer que le capteur est sur une ligne noire
// -----------------------------------------------------------------------------
// Declaration structs, unions, enums, bitfields, typedefs
// -----------------------------------------------------------------------------
// A corriger: utiliser la class pour stocker les valeurs des capteurs
//              et faire une class capteur, avec un héritage pour chaque type de capteur
struct sSensorValues 

{
    float fDistance1;    // Distance measured by the first sensor in cm 
    float fDistance2;    // Distance measured by the second sensor in cm 
    bool bC1IsOnTheLine; // True if the first line sensor is on the line
    bool bC2IsOnTheLine; // True if the second line sensor is on the line
    bool bC3IsOnTheLine; // True if the third line sensor is on the line
};

//------------------------------------------------------------------------------
// Declaration class                                           
//------------------------------------------------------------------------------
class UltrasonicSensors 
{
public:
    UltrasonicSensors(const uint8_t cu8TrigPin1, const uint8_t cu8EchoPin1, 
                        const uint8_t cu8TrigPin2, const uint8_t cu8EchoPin2);
    sSensorValues sReadDistances();

private:
    const uint8_t cu8TrigPin1;
    const uint8_t cu8EchoPin1;
    const uint8_t cu8TrigPin2;
    const uint8_t cu8EchoPin2;
    float fMeasureDistance(const uint8_t cu8TrigPin, const uint8_t cu8EchoPin);
};

class QRD1114Sensors
{
public:
    QRD1114Sensors(const uint8_t cu8AnalogPin1, const uint8_t cu8AnalogPin2, const uint8_t cu8AnalogPin3);
    bool bIsOnBlackLine(const uint8_t cu8AnalogPin);
    sSensorValues sReadLineSensors();

private:
    const uint8_t cu8AnalogPin1;
    const uint8_t cu8AnalogPin2;
    const uint8_t cu8AnalogPin3;
    float f32DummyProximity;
    float fReadProximity(const uint8_t cu8AnalogPin);
};

#endif // ROBOTPUTTERSENSORS_H
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

// -----------------------------------------------------------------------------
// Declaration structs, unions, enums, bitfields, typedefs
// -----------------------------------------------------------------------------
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

#endif // ROBOTPUTTERSENSORS_H
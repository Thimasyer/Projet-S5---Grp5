/**********************  COPMMENTAIRE DOXYGEN D'EN TETE-FICHIER ********************
 * @file UltrasonicSensors.cpp
 * @brief Definition for the header file Ultrasonics_Sensors.h 
 **********************************************************************************/
#include "RobotPutterSensors.h"
#include <stdint.h>

/**
* @brief Constructor for UltrasonicSensors
* @param cu8TrigPin1 Trigger pin for the first HC-SR04 sensor
* @param cu8EchoPin1 Echo pin for the first HC-SR04 sensor
* @param cu8TrigPin2 Trigger pin for the second HC-SR04 sensor
* @param cu8EchoPin2 Echo pin for the second HC-SR04 sensor
*/
UltrasonicSensors::UltrasonicSensors(const uint8_t cu8TrigPin1, 
                                    const uint8_t cu8EchoPin1, 
                                    const uint8_t cu8TrigPin2, 
                                    const uint8_t cu8EchoPin2) 
        : cu8TrigPin1(cu8TrigPin1), cu8EchoPin1(cu8EchoPin1), 
          cu8TrigPin2(cu8TrigPin2), cu8EchoPin2(cu8EchoPin2)
{
    pinMode(cu8TrigPin1, OUTPUT);
    pinMode(cu8EchoPin1, INPUT);
    pinMode(cu8TrigPin2, OUTPUT);
    pinMode(cu8EchoPin2, INPUT);
}

/**
* @brief Reads distances from the two sensors
* @return A SensorDistances structure containing the distances in cm
*/
sSensorValues UltrasonicSensors::sReadDistances() 
{
    sSensorValues sValues;
    sValues.fDistance1 = fMeasureDistance(cu8TrigPin1, cu8EchoPin1);
    delay(50);
    sValues.fDistance2 = fMeasureDistance(cu8TrigPin2, cu8EchoPin2);
    return sValues;
}

/**
 * @brief Measures the distance using the HC-SR04 sensor
 * @param cu8TrigPin The trigger pin for the sensor
 * @param cu8EchoPin The echo pin for the sensor
 * @return The distance measured in cm
 */
float UltrasonicSensors::fMeasureDistance(const uint8_t cu8TrigPin, 
                                        const uint8_t cu8EchoPin) 
{
    float fDistance = 0.0;
    digitalWrite(cu8TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(cu8TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(cu8TrigPin, LOW);
    // Mesurer la durée du signal Echo
    uint16_t u16Duration = pulseIn(cu8EchoPin, HIGH);
    #if DEBUG
    Serial.print("Duration: ");
    Serial.println(u16Duration);  // Vérifie la durée du signal mesuré
    #endif
     // Calcul de la distance en cm
    fDistance = u16Duration * 0.034 / 2; // Conversion en cm
    
    return fDistance;
}
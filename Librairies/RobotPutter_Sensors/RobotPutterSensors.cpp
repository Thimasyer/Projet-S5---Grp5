/**********************  COPMMENTAIRE DOXYGEN D'EN TETE-FICHIER ********************
 * @file UltrasonicSensors.cpp
 * @brief Definition for the header file Ultrasonics_Sensors.h 
 * 
 **********************************************************************************/
#include "RobotPutterSensors.h"
#include <stdint.h>

/**
* @fn UltrasonicSensors::UltrasonicSensors(const uint8_t cu8TrigPin1, 
                                    const uint8_t cu8EchoPin1, 
                                    const uint8_t cu8TrigPin2, 
                                    const uint8_t cu8EchoPin2) 
        : cu8TrigPin1(cu8TrigPin1), cu8EchoPin1(cu8EchoPin1), 
          cu8TrigPin2(cu8TrigPin2), cu8EchoPin2(cu8EchoPin2)
* @brief Constructor for UltrasonicSensors
* @param cu8TrigPin1 Trigger pin for the first HC-SR04 sensor
* @param cu8EchoPin1 Echo pin for the first HC-SR04 sensor
* @param cu8TrigPin2 Trigger pin for the second HC-SR04 sensor
* @param cu8EchoPin2 Echo pin for the second HC-SR04 sensor
* @callergraph
* @callgraph
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
* @fn sSensorValues UltrasonicSensors::sReadDistances() 
* @brief Reads distances from the two sensors
* @return A SensorDistances structure containing the distances in cm
* @callergraph
* @callgraph
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
 * @fn float UltrasonicSensors::fMeasureDistance(const uint8_t cu8TrigPin, 
                                            const uint8_t cu8EchoPin)
 * @brief Measures the distance using the HC-SR04 sensor
 * @param cu8TrigPin The trigger pin for the sensor
 * @param cu8EchoPin The echo pin for the sensor
 * @return The distance measured in cm
 * @callergraph
 * @callgraph
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

/**
 * @brief Construct a new QRD1114Sensor::QRD1114Sensor object
 * @param cu8AnalogPin1
 * @param cu8AnalogPin2
 * @param cu8AnalogPin3
 * @note Reading from front left to right
 * Sensor from front left is on pin1, sensor from front right is on pin2, sensor from back is on pin3. 
 * 
 */
QRD1114Sensors::QRD1114Sensors(const uint8_t cu8AnalogPin1, 
                                const uint8_t cu8AnalogPin2, 
                                const uint8_t cu8AnalogPin3)
        : cu8AnalogPin1(cu8AnalogPin1), 
        cu8AnalogPin2(cu8AnalogPin2), 
        cu8AnalogPin3(cu8AnalogPin3)
{
    pinMode(cu8AnalogPin1, INPUT);
    pinMode(cu8AnalogPin2, INPUT);
    pinMode(cu8AnalogPin3, INPUT);
}

/**
 * @brief Read the proximity value from the sensor
 * @return float The proximity value in volts
 */
float QRD1114Sensors::fReadProximity(const uint8_t cu8AnalogPin) 
{
    int32_t i32ProximityADC = analogRead(cu8AnalogPin);
    #if DEBUG
    Serial.print("Proximity ADC: ");
    Serial.println(i32ProximityADC);
    #endif
    return  (static_cast<float>(i32ProximityADC) * 5.0 / 1023.0) ;
}

/**
 * @brief Check if the sensor is on a black line
 * @return true The sensor is on a black line
 * @return false The sensor is not on a black line
 */
bool QRD1114Sensors::bIsOnBlackLine(const uint8_t cu8AnalogPin)
{
    // Variable pour return le résultat
    bool bResult;
    float f32Proximity = fReadProximity(cu8AnalogPin);
    // N'évite pas les faux positifs
    if (f32Proximity > LINE_TRESHOLD)
    {
        bResult = true;
    }
    else
    {
        bResult = false;
    }
    #if DEBUG
    if (bResult)
    {
        Serial.print("Capteur IR ");
        Serial.print(cu8AnalogPin);
        Serial.print(" est sur une ligne noire");
    }
    else 
    {
        Serial.print("Capteur IR ");
        Serial.print(cu8AnalogPin);
        Serial.print(" n'est pas sur une ligne noire");
    }
    #endif
    return bResult;
}

/**
 * @brief Read the values of the 3 line sensors
 * @return sSensorValues The struct which pack sensors data
 */
sSensorValues QRD1114Sensors::sReadLineSensors()
{
    sSensorValues sValues;
    sValues.bC1IsOnTheLine = bIsOnBlackLine(PinIR1);
    sValues.bC2IsOnTheLine = bIsOnBlackLine(PinIR2);
    sValues.bC3IsOnTheLine = bIsOnBlackLine(PinIR3);
    return sValues;
}


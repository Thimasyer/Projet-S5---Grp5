/**************************************************************************
 * @file BL_HC05.h
 * @author Thomas Eyer
 * @brief Header file for handling Bluetooth communication with HC05
 * @version 1.1
 * @date 2025-01-09
 * 
 * @copyright Copyright (c) 2025
 * 
 **************************************************************************/
#ifndef BL_FUNCTIONS_H
#define BL_FUNCTIONS_H

// -----------------------------------------------------------------------------
// Lib packages includes, compiler define
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <String.h>
#include "RobotPutterSensors.h"
#include "State.h"

// Define default RX/TX pins for Bluetooth
#define HC05_RX_PIN 3
#define HC05_TX_PIN 2


// -----------------------------------------------------------------------------
// Declaration structs, unions, enums, bitfields, typedefs
// -----------------------------------------------------------------------------
extern StateOfRobot eStateOfRobot;
extern sSensorValues sSensorData;
//------------------------------------------------------------------------------
// Declaration classes                                          
//------------------------------------------------------------------------------
class BLFunctions
{
private:
    bool bCommandAvailable;
    // Attributes for parsed commands
    uint16_t ui16AdjustHeight;
    uint16_t ui16AdjustAngle;
    StateOfRobot eProchainEtat = eInit;

    // Parse incoming Bluetooth data and update command attributes
    void vParseData(const String& sData);

public:
    //  A REDEPLACER 
    SoftwareSerial oBTSerial;
    String sBuffer; // Buffer for incoming data
    // Constructor 
    BLFunctions(uint8_t iRxPin = HC05_RX_PIN, uint8_t iTxPin = HC05_TX_PIN, 
                                            uint32_t iBaudRate = 38400);
    void vSendSensorData();
    void vReceiveData();
    // Getters for BL data
    uint16_t ui16GetAdjustHeight() const;
    uint16_t ui16GetAdjustAngle() const;
    StateOfRobot eGetNextState() const;
    bool bIsCommandAvailable() const;
    void vClearCommandFlag();

    // Setter for BL data
    void vSetAdjustHeight(uint16_t iHeight);
    void vSetAdjustAngle(uint16_t iAngle);
    void vSetNextState(StateOfRobot eNextState);
};

#endif // BL_FUNCTIONS_H

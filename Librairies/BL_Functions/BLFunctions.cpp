/************************************************************
 * @file BL_HC05.cpp
 * @author Thomas Eyer
 * @brief Handle Bluetooth communication with HC05
 * @version 1.1
 * @date 2025-01-09
 * 
 * @copyright Copyright (c) 2025
 * 
 ************************************************************/
#include "BLFunctions.h"

/**
 * @brief Constructor to initialize the Bluetooth module with given RX/TX pins and baud rate.
 * @param iRxPin: RX pin for the HC-05 module.
 * @param iTxPin: TX pin for the HC-05 module.
 * @param iBaudRate: Baud rate for communication.
 */
BLFunctions::BLFunctions(uint8_t iRxPin, uint8_t iTxPin, uint32_t iBaudRate)
                : oBTSerial(iRxPin, iTxPin), 
                    bCommandAvailable(false), 
                    ui16AdjustHeight(0), 
                    ui16AdjustAngle(0), 
                    eProchainEtat(eInit)
{
    oBTSerial.begin(iBaudRate);
    sBuffer.reserve(64); // Reserve buffer memory
}

/**
 * @brief Send sensor data and robot state via Bluetooth.
 */
void BLFunctions::vSendSensorData()
{
    // format sensor data from the struct in one string
    String strSensorData = String(sSensorData.fDistance1) + ";" + 
                         String(sSensorData.fDistance2) + ";" + 
                         String(sSensorData.bC1IsOnTheLine) + ";" + 
                         String(sSensorData.bC2IsOnTheLine) + ";" + 
                         String(sSensorData.bC3IsOnTheLine) + ";" + 
                         String(eStateOfRobot);
                         String("1");
                         String("2") + "\n";
    oBTSerial.println(strSensorData);
}

/**
 * @brief Check for new data from Bluetooth and process it if available.
 */
void BLFunctions::vReceiveData()
{
    while (oBTSerial.available())
    {
        char c = oBTSerial.read();
        #if DEBUG
        Serial.print(c);
        #endif
        if (c == '#') // End of message
        {
            #if DEBUG
            Serial.print("La chaine complète:");
            Serial.println(sBuffer);
            #endif
            vParseData(sBuffer);
            sBuffer = ""; // Clear buffer
            bCommandAvailable = true;
        }
        else
        {
            sBuffer += c; // Append character to buffer
        }
    }
}

/**
 * @brief Parse incoming Bluetooth data and update command attributes.
 * @param sData: String containing Bluetooth data.
 */
void BLFunctions::vParseData(const String& sData)
{
    // Returns the position of the first occurrence of ';' in the string
    int iDelim1 = sData.indexOf(';');
    // Searrch the first occurrence of ';' at the position iDelim1 + 1
    int iDelim2 = sData.indexOf(';', iDelim1 + 1);
    // Same logic as above
    int iDelim3 = sData.indexOf('#', iDelim2 + 1);

    // indexOf return -1 if not found
    if (iDelim1 != -1 && iDelim2 != -1)
    {
        ui16AdjustHeight = sData.substring(0, iDelim1).toInt();
        ui16AdjustAngle = sData.substring(iDelim1 + 1, iDelim2).toInt();
        eProchainEtat = static_cast<StateOfRobot>(sData.substring(iDelim2 + 1, iDelim3).toInt());
    }
}

/**
 * @brief Get the height adjustment command.
 * @return The height adjustment as a 16-bit unsigned integer.
 */
uint16_t BLFunctions::ui16GetAdjustHeight() const
{
    return ui16AdjustHeight;
}

/**
 * @brief Get the angle adjustment command.
 * @return The angle adjustment as a 16-bit unsigned integer.
 */
uint16_t BLFunctions::ui16GetAdjustAngle() const
{
    return ui16AdjustAngle;
}

/**
 * @brief Get the next robot state.
 * @return The next state of the robot as an enum value.
 */
StateOfRobot BLFunctions::eGetNextState() const
{
    return eProchainEtat;
}

/**
 * @brief Check if a new command is available.
 * @return True if a new command has been received.
 */
bool BLFunctions::bIsCommandAvailable() const
{
    return bCommandAvailable;
}

/**
 * @brief Clear the command availability flag.
 */
void BLFunctions::vClearCommandFlag()
{
    bCommandAvailable = false;
}

void BLFunctions::vSetNextState(StateOfRobot eState)
{
    eProchainEtat = eState;
}

void BLFunctions::vSetAdjustHeight(uint16_t ui16Height)
{
    ui16AdjustHeight = ui16Height;
}

void BLFunctions::vSetAdjustAngle(uint16_t ui16Angle)
{
    ui16AdjustAngle = ui16Angle;
}

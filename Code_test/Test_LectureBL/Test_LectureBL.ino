// #include <Arduino.h>
// #include "BLFunctions.h"
// #include "RobotPutterSensors.h"
// #include "State.h"

// // Create a global Bluetooth object

// StateOfRobot eStateOfRobot  = eInit;
// sSensorValues sSensorData; 
// BLFunctions oBluetooth(3,2);

// void setup()
// {
//   Serial.begin(9600);
//   // Activation de l'entrée Key du HC05
//   pinMode(9,OUTPUT) ;
//   digitalWrite(9,HIGH) ;
//   sSensorData.bC1IsOnTheLine = 1;
//   sSensorData.bC2IsOnTheLine = 1;
//   sSensorData.bC3IsOnTheLine = 1;
//   sSensorData.fDistance1 = 50;
//   sSensorData.fDistance2 = 30;
// }

// void loop()
// {
//     // Example: Sending sensor data
//     oBluetooth.vSendSensorData();

//     // Example: Receiving data
//     //oBluetooth.vReceiveData();
//     while (oBluetooth.oBTSerial.available())
//     {
//         char c = oBluetooth.oBTSerial.read();
//         Serial.print(c);
//         if (c == '\n') // End of message
//         {
//             oBluetooth.sBuffer = ""; // Clear buffer
//             Serial.print("La chaine complète:");
//             Serial.println(oBluetooth.sBuffer);
//         }
//         else
//         {
//             oBluetooth.sBuffer += c; // Append character to buffer
//             //Serial.println("En cours");
//         }
//     }
//     if (oBluetooth.bIsCommandAvailable())
//     {
//         Serial.print("Command 1: ");
//         Serial.println(oBluetooth.ui16GetAdjustHeight());
//         Serial.print("Command 2: ");
//         Serial.println(oBluetooth.ui16GetAdjustAngle());
//         Serial.print("Command 3: ");
//         Serial.println(oBluetooth.eGetNextState());

//         // Clear the flag after processing
//         oBluetooth.vClearCommandFlag();
//     }
//     delay(100);
// }

#include <Arduino.h>
#include "BLFunctions.h"
#include "State.h"

#define TIME_MEASURE 1
#define DEBUG 1

#if TIME_MEASURE
uint16_t ui16Dummy;
uint64_t ui64Start, ui64End;
#endif
BLFunctions oBluetooth(3,2);
StateOfRobot eStateOfRobot  = eInit;
sSensorValues sSensorData;  

void setup()
{
  Serial.begin(9600);
  // Activation de l'entrée Key du HC05
  pinMode(9,OUTPUT) ;
  digitalWrite(9,HIGH) ;

  sSensorData.bC1IsOnTheLine = 1;
  sSensorData.bC2IsOnTheLine = 1;
  sSensorData.bC3IsOnTheLine = 1;
  sSensorData.fDistance1 = 50;
  sSensorData.fDistance2 = 30;
}

void loop()
{
  oBluetooth.vSendSensorData();
   // Mesure du temps
    #if TIME_MEASURE
    ui64Start = millis();
    #endif
    oBluetooth.vReceiveData();
    if (oBluetooth.ui16GetAdjustHeight() != 0)
    {
        Serial.print("Adjusting height by: ");
        Serial.println(oBluetooth.ui16GetAdjustHeight());
        oBluetooth.vSetAdjustHeight(0);   // Reset
    }
    if (oBluetooth.ui16GetAdjustAngle() != 0)
    {
        Serial.print("Adjusting angle by: ");
        Serial.println(oBluetooth.ui16GetAdjustAngle());
        oBluetooth.vSetAdjustAngle(0);   // Reset
    }
    if (oBluetooth.eGetNextState() != eInit)
    {
        Serial.print("Next state: ");
        Serial.println(oBluetooth.eGetNextState());
        oBluetooth.vSetNextState(eInit);   // Reset
    }

    // Mesure du temps, et affichage
    if ((ui16Dummy%10)== 0)
    {
        #if TIME_MEASURE
        // Deuxième mesure du temps
        ui64End = millis();
        // Calcul du temps écoulé
        uint32_t ui32ElapsedTime= (uint32_t)(ui64End - ui64Start);
        Serial.println("Temps d'exécution (en ms): ");
        Serial.println(ui32ElapsedTime);
        #endif
    }
    ui16Dummy++;
}

// #include <Arduino.h>
// #include "RobotPutterSensors.h"
// #include "MotorControl.h"

// // Définition des constantes preprocesseur pour debogage
// #define DEBUG 1                // Définir à 1 pour activer le mode debug
// #define TIME_MEASURE 1         // Définir à 1 pour activer le mode mesure de temps

// #include "AlgoRobotPutter.h"  // inclusion après définitio des constante prepoc

// // ------------------ Variables globales --------------------
// #if TIME_MEASURE
// uint16_t ui16Dummy;
// uint64_t ui64Start, ui64End;
// #endif
// // Structure pour stocker les valeurs des capteurs
// sSensorValues sDistancesParois;
// bool bDummy;

// void setup() 
// {
//   Serial.begin(9600); 
//   vMotorDriverInit(PinDir2, PinPWM2, PinDir1, PinPWM1);
//   #if DEBUG
//   Serial.println("Initialisations terminé, prêt.");
//   #endif
// }

// void loop() 
// {
//   // ###############  Mesure du temps ##################
//   #if TIME_MEASURE
//   ui64Start = millis();
//   #endif
//   // ###################################################
//   if (bDummy)
//   {
//     vFollowLine();
//     #if DEBUG
//     Serial.println("Ligne suivie jusqu'au T");
//     #endif
//   }
//   bDummy = 1;
// }

#include <Arduino.h>
#define DEBUG 1                // Définir à 1 pour activer le mode debug
#include "RobotPutterSensors.h"
#include "MotorControl.h"

// Définition des constantes preprocesseur pour debogage
#define TIME_MEASURE 1         // Définir à 1 pour activer le mode mesure de temps

// ------------------ Variables globales --------------------
#if TIME_MEASURE
uint16_t ui16Dummy;
uint64_t ui64Start, ui64End;
#endif
UltrasonicSensors oCapteurUS(PinUsTrig, PinUsEchoDroit, 
                            PinUsTrig, PinUsEchoArriere);
QRD1114Sensors oCapteurIR(PinIR1, PinIR2, PinIR3);
// Structure pour stocker les valeurs des capteurs
sSensorValues sDistancesParois;
bool bDummy = 1;
// ------------------- SETUP ----------------------------------
void setup() 
{
  Serial.begin(9600); 
  vMotorDriverInit(PinDir2, PinPWM2, PinDir1, PinPWM1);
  #if DEBUG
  Serial.println("Initialisations terminé, prêt.");
  #endif
}

// ------------------------- LOOP ---------------------------
void loop() 
{
  // ###############  Mesure du temps ##################
  #if TIME_MEASURE
  ui64Start = millis();
  #endif
  // ###################################################
  bool bIsOnT = 0;
  bool bC1TouchedTheTFirst = 0;
  sSensorValues sSensorIR;
  // Tant que les deux capteurs IR ne sont pas sur la ligne en même temps
  while (bIsOnT == 0)
  {
      sSensorIR = oCapteurIR.sReadLineSensors();
      // Différencation des cas, quels capteurs détecte la ligne
      if (sSensorIR.bC1IsOnTheLine && (!sSensorIR.bC2IsOnTheLine))
      {
        #if DEBUG
        Serial.println("C1 touche");
        #endif
        vSetMotor(MOTOR_RIGHT, FORWARD, 180); // Avance moteur droit
        vStopMotor(MOTOR_LEFT);               // Arrêt moteur gauche
        bC1TouchedTheTFirst = 1;
      }
      if (sSensorIR.bC2IsOnTheLine && (!sSensorIR.bC1IsOnTheLine))
      {
        #if DEBUG
        Serial.println("C2 touche");
        #endif
        vSetMotor(MOTOR_LEFT, FORWARD, 180);  // Avance moteur gauche
        vStopMotor(MOTOR_RIGHT);              // Arrêt moteur droit
        bC1TouchedTheTFirst = 0;
      }
      if ((!sSensorIR.bC1IsOnTheLine) && (!sSensorIR.bC2IsOnTheLine))
      {
        #if DEBUG
        Serial.println("Aucun ne touchent");
        #endif
        vSetMotor(MOTOR_LEFT, FORWARD, 220); // Avance moteur gauche
        vSetMotor(MOTOR_RIGHT, FORWARD, 220); // Avance moteur droit
      }
      if (sSensorIR.bC1IsOnTheLine && sSensorIR.bC2IsOnTheLine)
      {
        #if DEBUG
        Serial.println("Les deux touchent");
        #endif
        bIsOnT = 1;
      }
    }
}

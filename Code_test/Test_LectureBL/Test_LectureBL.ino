// #include <Arduino.h>
// #include "State.h"
// #include <SoftwareSerial.h>
// #include "BLFunctions.h"

// // Définition des pins RX/TX pour le HC05, 
// #define HC05_RX_PIN 2
// #define HC05_TX_PIN 3
// #define TIME_MEASURE 0
// #define DEBUG 1

// // Structure pour les données des capteurs
// struct sSensorValues {
//     float fDistance1;
//     float fDistance2;
//     bool bC1IsOnTheLine;
//     bool bC2IsOnTheLine;
//     bool bC3IsOnTheLine;
// };

// // Enum pour l'état du robot
// StateOfRobot eStateOfRobot = eInit;
// sSensorValues sSensorData;

// SoftwareSerial BTSerial(HC05_RX_PIN, HC05_TX_PIN);  // Communication Bluetooth

// String sBuffer; // Buffer pour stocker les données reçues

// // Fonction pour envoyer les données des capteurs via Bluetooth
// void vSendSensorData() {
//     String strSensorData = String(sSensorData.fDistance1) + ";" + 
//                            String(sSensorData.fDistance2) + ";" + 
//                            String(sSensorData.bC1IsOnTheLine) + ";" + 
//                            String(sSensorData.bC2IsOnTheLine) + ";" + 
//                            String(sSensorData.bC3IsOnTheLine) + ";" + 
//                            String(eStateOfRobot) + "\n";
//     BTSerial.println(strSensorData);
// }

// // Fonction pour recevoir des données via Bluetooth
// void vReceiveData() {
//     while (BTSerial.available()) 
//     {
//         char c = BTSerial.read();
//         #if DEBUG
//         Serial.print(c);
//         #endif
//         if (c == '#') { // Fin de message
//             // Traitement des données reçues
//             Serial.print("La chaine complète:");
//             Serial.print(sBuffer);
//             vParseData(sBuffer);
//             sBuffer = ""; // Effacer le buffer
//         } else {
//             sBuffer += c; // Ajouter caractère au buffer
//         }

//     }
// }

// // Fonction pour analyser les données reçues
// void vParseData(const String& sData) 
// {
//   Serial.print("Buffer reçu");
//   Serial.println(sData);
//   Serial.println("");
//     int iDelim1 = sData.indexOf(';');
//     int iDelim2 = sData.indexOf(';', iDelim1 + 1);
//     int iDelim3 = sData.indexOf('#', iDelim2 + 1);
//   Serial.print(iDelim1);
//   Serial.print(iDelim2);
//   Serial.print(iDelim3);

//     if ((iDelim1 != -1) && (iDelim2 != -1)) 
//     {
//       #if DEBUG
//       Serial.println("Delim OK");
//       #endif
//         // Extraire les données de la chaîne et les convertir
//         uint16_t ui16AdjustHeight = sData.substring(0, iDelim1).toInt();
//         uint16_t ui16AdjustAngle = sData.substring(iDelim1 + 1, iDelim2).toInt();
//         eStateOfRobot = static_cast<StateOfRobot>(sData.substring(iDelim2 + 1, iDelim3).toInt());

//         #if DEBUG
//         // Traitement des données extraites
//         Serial.print("Adjust Height: ");
//         Serial.println(ui16AdjustHeight);
//         Serial.print("Adjust Angle: ");
//         Serial.println(ui16AdjustAngle);
//         Serial.print("Next State: ");
//         Serial.println(eStateOfRobot);
//         #endif
//     }
// }

// // Fonction d'initialisation
// void setup() {
//     Serial.begin(9600);
//     BTSerial.begin(38400); // Initialiser Bluetooth
//     // Initialisation des données des capteurs
//     sSensorData.bC1IsOnTheLine = 1;
//     sSensorData.bC2IsOnTheLine = 1;
//     sSensorData.bC3IsOnTheLine = 1;
//     sSensorData.fDistance1 = 50.0;
//     sSensorData.fDistance2 = 30.0;
// }

// // Fonction de boucle principale
// void loop() {
//     // Envoi des données des capteurs
//     vSendSensorData();

//     // Mesure du temps (optionnel)
//     #if TIME_MEASURE
//     uint64_t ui64Start = millis();
//     #endif

//     // Réception des données Bluetooth
//     vReceiveData();


//     // Mesure du temps, et affichage
//     #if TIME_MEASURE
//     uint64_t ui64End = millis();
//     uint32_t ui32ElapsedTime = (uint32_t)(ui64End - ui64Start);
//     Serial.print("Execution Time (ms): ");
//     Serial.println(ui32ElapsedTime);
//     #endif

//     delay(100);  // Délai entre les cycles
// }

#include "BLFunctions.h"
#include "State.h"
#define DEBUG 1

// Déclaration de l'objet Bluetooth sur les pins RX 10, TX 11
// RX et TX à brancher en croisé
BLFunctions btModule(2, 3);  
StateOfRobot eStateOfRobot = eInit;
sSensorValues sSensorData;

void setup() 
{
  Serial.begin(9600);  // Initialisation du port série pour la communication avec le moniteur série
  #if DEBUG
  Serial.println("Initialisations terminé, prêt.");
  #endif
}

void loop() 
{
  btModule.vReceiveData();
  // Vérifie s'il y a une nouvelle commande disponible depuis Bluetooth
  if (btModule.eGetNextState() != eInit)
  {
      eStateOfRobot = btModule.eGetNextState();
      Serial.println(eStateOfRobot);
      btModule.vSetNextState(eInit); // remise à zéro
  }
   if (btModule.ui16GetAdjustAngle() != 0)
  {
    Serial.print("Angle: ");
    Serial.println(btModule.ui16GetAdjustAngle());
    btModule.vSetAdjustAngle(0);   // remise à zéro
  }
  // ajuster la hauteur de la plaque
  if (btModule.ui16GetAdjustHeight() != 0)
  {
    Serial.print("Hauteur: ");
    Serial.println(btModule.ui16GetAdjustHeight());
    btModule.vSetAdjustHeight(0); 
  }
  
  // Ajoutez un délai pour ne pas saturer la boucle
  delay(100);
}

/***********  COMMENTAIRE DOXYGEN D'EN TETE-FICHIER ***************************
 * @file Test_CapteurIR.ino
 * @brief Programme pour tester le capteur QRD1114.
 * Ce programme initialise un capteur QRD1114 et vérifie s'il est
 * sur une ligne noire. Les résultats sont affichés dans le Moniteur Série.
 Contient 2 Test, choisir lequel compiler en définissant TEST1 et TEST2
 ******************************************************************************/
 #define TEST1 1
 #define TEST2 0
 
 #if TEST1 // *******************************************************************
const int QRD1114_PIN = A2; // Sensor output voltage

void setup() 
{
  Serial.begin(9600);
  pinMode(QRD1114_PIN, INPUT);
}

void loop() 
{
  // Read in the ADC and convert it to a voltage:
  int proximityADC = analogRead(QRD1114_PIN);
  float proximityV = (float)proximityADC * 5.0 / 1023.0;
  Serial.println(proximityV);
  delay(100);
}
#endif // TEST1

#if TEST2 // ****************************************************************
#include "RobotPutterSensors.h"
#define DEBUG 1
// Déclaration du capteur QRD1114 sur la broche analogique A0
const uint8_t cu8CapteurIRPin = A2;
QRD1114Sensors oCapteurIR(cu8CapteurIRPin, A3, A4);

/**
 * @brief Configuration initiale Arduino
 * Initialise le capteur QRD1114 et le Moniteur Série.
 */
void setup() 
{
    Serial.begin(9600);
    Serial.println("Test du capteur QRD1114 commence...");
}

/**
 * @brief Boucle principale
 * Lit la valeur de proximité et vérifie si le capteur détecte une ligne noire.
 */
void loop() 
{
    if (oCapteurIR.bIsOnBlackLine(cu8CapteurIRPin)) 
    {
        Serial.println("Le capteur 1 est sur une ligne noire !");
    } else {
        Serial.println("Le capteur 1 n'est pas sur une ligne noire.");
    }
    if (oCapteurIR.bIsOnBlackLine(A3)) 
    {
        Serial.println("Le capteur 2 est sur une ligne noire !");
    } else {
        Serial.println("Le capteur 2 n'est pas sur une ligne noire.");
    }
        if (oCapteurIR.bIsOnBlackLine(A4)) 
    {
        Serial.println("Le capteur 3 est sur une ligne noire !");
    } else {
        Serial.println("Le capteur 3 n'est pas sur une ligne noire.");
    }


    delay(500); // Délai pour pouvoir lire les valeurs sur le moniteur
}
#endif // TEST2
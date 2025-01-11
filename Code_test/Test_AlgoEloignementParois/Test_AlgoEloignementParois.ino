/**
 * @file Test_AlgoEloignementParois.ino
 * @author Thomas Eyer
 * @brief Code Arduino qui permet de tester l'algorithme d'éloignement des parois
 * @version 0.1
 * @date 2025-01-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include <Arduino.h>
#include "RobotPutterSensors.h"
#include "MotorControl.h"
#include <stdint.h>             // pour les types de données uint32_t etc.

// Définition des pins des capteurs ultrasons
#define PIN_US_TRIG 13
#define PIN_US_ECHO_DROIT 2      // Droite
#define PIN_US_ECHO_ARRIERE 4    // Arrière
// Définition des pins des moteurs (PW M et direction)
#define DIR_PIN_L 7
#define PWM_PIN_L 9
#define DIR_PIN_R 8
#define PWM_PIN_R 10
// Définition des constantes
#define ROBOT_WIDTH_CM 20      // Largeur du robot en cm
#define MIN_RIGHT_DIST_CM 8    // Distance minimale droite avant rotation
#define MAX_RIGHT_DIST_CM 50   // Distance maximale droite avant rotation
#define MAX_BACK_DIST_CM 50    // Distance maximale avant arrêt
#define TURN_TIME_MS 100       // Durée de rotation en ms
// Définition DEBUG
#define DEBUG 0
#define TIME_MEASURE 1 

// Initialisation des capteurs ultrasoniques
UltrasonicSensors ultrasonicSensors(PIN_US_TRIG, PIN_US_ECHO_DROIT, 
                                     PIN_US_TRIG, PIN_US_ECHO_ARRIERE);

typedef double f64_t;
uint16_t uiDummy = 0;
#if TIME_MEASURE
uint64_t ui64Start, ui64End;
#endif

/**
 * @brief Fonctions d'initialisation Arduino
 * @callergraph
 * @callgraph
 * 
 */
void setup() 
{
    // Initialisation du moniteur série
    Serial.begin(9600);
    // Initialisation des moteurs
    vMotorDriverInit(DIR_PIN_L, PWM_PIN_L, DIR_PIN_R, PWM_PIN_R);
    #if DEBUG
    Serial.println("Initialisations terminé, prêt.");
    #endif
}

/**
 * @brief Fonction principale Loop d'Arduino
 * @callergraph
 * @callgraph
 * 
 */
void loop() 
{
    #if TIME_MEASURE
    // Mesure du temps
    ui64Start = millis();
    #endif
    // Lire les distances des capteurs ultrasoniques
    sSensorValues sensorValues = ultrasonicSensors.sReadDistances();
    float fDistDroite = sensorValues.fDistance1;            // Droit
    float fDistAuMurArriere = sensorValues.fDistance2;      // Arrière
    // Log distances pour débogage
    #if DEBUG
    Serial.print("Distance droit : ");
    Serial.print(fDistDroite);
    Serial.print(" cm, Distance arrière : ");
    Serial.print(fDistAuMurArriere);
    Serial.println(" cm");
    #endif

    // Comportement en fonction des distances
    if (fDistAuMurArriere > MAX_BACK_DIST_CM) 
    {
        #if DEBUG
        Serial.println("Distance arrière trop grande. Arrêt du robot.");
        delay(1000);
        #endif
        vStopMotor(MOTOR_LEFT);
        vStopMotor(MOTOR_RIGHT);
    } 
    else if (fDistDroite < MIN_RIGHT_DIST_CM) 
    {
        #if DEBUG
        Serial.println("Distance droite trop faible. Rotation vers la gauche.");
        delay(1000);
        #endif
        vStopMotor(MOTOR_LEFT);              // Arrêt moteur droit
        vSetMotor(MOTOR_RIGHT, FORWARD, 200); // Recule moteur gauche
        delay(TURN_TIME_MS);                  // Pause pour permettre la rotation
        vStopMotor(MOTOR_RIGHT);               // Arrêt moteur gauche
    } 
    else if (fDistDroite > MAX_RIGHT_DIST_CM) 
    {
        #if DEBUG
        Serial.println("Distance droite trop grande. Rotation vers la droite.");
        delay(1000);
        #endif
        vStopMotor(MOTOR_RIGHT);             // Arrêt moteur gauche
        vSetMotor(MOTOR_LEFT, FORWARD, 200); // Recule moteur droit
        delay(TURN_TIME_MS); // Pause pour permettre la rotation
        vStopMotor(MOTOR_LEFT); // Arrêt moteur droit
    } 
    else 
    {
        #if DEBUG
        Serial.println("Zone dégagée. Avance du robot.");
        delay(1000);
        #endif
        if(uiDummy%2 == 0)
        {
            vSetMotor(MOTOR_LEFT, FORWARD, 250); // Avance moteur gauche
            vSetMotor(MOTOR_RIGHT, FORWARD, 250); // Avance moteur droit
        }
        else
        {
            vSetMotor(MOTOR_RIGHT, FORWARD, 250); // Avance moteur droit
            vSetMotor(MOTOR_LEFT, FORWARD, 250); // Avance moteur gauche
        }
    }
}


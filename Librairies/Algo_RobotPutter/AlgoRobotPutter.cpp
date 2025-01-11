/**
 * @file AlgoRobotPutter.cpp
 * @author Thomas Eyer
 * @brief Fichier de défintion des algorithmes du robot putter
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "AlgoRobotPutter.h"

// Instanciation des objets globales, 
// ATTENTION: un .h n'allouera jamais de mémoire pour un objet
UltrasonicSensors oCapteurUS(PinUsTrig, PinUsEchoDroit, 
                            PinUsTrig, PinUsEchoArriere);
QRD1114Sensors oCapteurIR(PinIR1, PinIR2, PinIR3);

/**
 * @brief Fonction pour rechercher la ligne
 * @return void
 * @note Fonction boucle qui permet de rechercher la ligne 
 * en effectuant un circuit entre 3 murs, jusqu'a ce qu'un des
 * deux capteurs IR avant détecte la ligne.
 */
void vSearchLine(void)
{
    bool bLineDetected = false;
    sSensorValues sSensorUsAndIr;
    uint16_t ui16Dummy = 0;
    while(bLineDetected)
    {
        // Lecture des capteurs ultrasoniques 
        // (pas opti le recopiage dans 2 struct, puis copié l'une dans l'autre)
        // (Mais le PB c'est qu'une fonction me lit seulement fDistance1 et 2)
        // (tandis que l'autre me lit les bC1IsOnTheLine, bC2IsOnTheLine et fDistance1 et 2)
        sSensorValues sDummy = oCapteurUS.sReadDistances();
        sSensorUsAndIr = oCapteurIR.sReadLineSensors();
        sSensorUsAndIr.fDistance1 = sDummy.fDistance1;
        sSensorUsAndIr.fDistance2 = sDummy.fDistance2;
        // Ligne détectée ?
        if (sSensorUsAndIr.bC1IsOnTheLine || sSensorUsAndIr.bC2IsOnTheLine)
        {
            bLineDetected = true;
        }
        // Comportement en fonction des distances
        if (sSensorUsAndIr.fDistance2 > MAX_BACK_DIST_CM)          //fDistance2 = arrière
        {
            #if DEBUG
            Serial.println("Distance arrière trop grande. Arrêt du robot.");
            delay(1000);
            #endif
            vStopMotor(MOTOR_LEFT);
            vStopMotor(MOTOR_RIGHT);
        } 
        else if (sSensorUsAndIr.fDistance1 < MIN_RIGHT_DIST_CM)    //fDistance1 = droite
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
        else if (sSensorUsAndIr.fDistance1 > MAX_RIGHT_DIST_CM) 
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
            if(ui16Dummy%2 == 0)
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
}

/**
 * @brief Fonction pour suivre la ligne
 * @param none
 * @return bReturn: 1 si OK, 0 si erreur
 * @note Fonction qui positionne le robot sur la ligne
 */
bool bStepOnLine(void)
{ 
    bool bReturn = 1;
    sSensorValues sSensorIR;
    sSensorIR = oCapteurIR.sReadLineSensors();
    // Différencation des cas, quels capteurs à détecter la ligne
    if (sSensorIR.bC1IsOnTheLine)           //c1 = capteur avant gauche
    {
        while(!sSensorIR.bC2IsOnTheLine)     //c2 = capteur avant droit
        {
            vSetMotor(MOTOR_LEFT, FORWARD, 180); // Avance moteur gauche
            vSetMotor(MOTOR_RIGHT, FORWARD, 180); // Avance moteur droit
            delay(50);
            vStopMotor(MOTOR_LEFT);
            vStopMotor(MOTOR_RIGHT);
            sSensorIR = oCapteurIR.sReadLineSensors();
        }
    }
    if (sSensorIR.bC2IsOnTheLine)
    {
        while(!sSensorIR.bC1IsOnTheLine)     //c1 = capteur avant gauche
        {
            vSetMotor(MOTOR_LEFT, FORWARD, 180); // Avance moteur gauche
            vSetMotor(MOTOR_RIGHT, FORWARD, 180); // Avance moteur droit
            delay(50);
            vStopMotor(MOTOR_LEFT);
            vStopMotor(MOTOR_RIGHT);
            sSensorIR = oCapteurIR.sReadLineSensors();
        }
    }
    else
    {
        bReturn = 0;
        #if DEBUG
        Serial.println("ERREUR: Les deux capteurs IR sont sur la ligne");
        #endif
    }
    return bReturn;
}

/**
 * @brief Fonction pour suivre la ligne
 * @param none
 * @return void
 * @note S'arrête si les deux capteurs IR détecte la ligne noir
 */
bool vFollowLine(void)
{
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
            vStopMotor(MOTOR_LEFT);
            vStopMotor(MOTOR_RIGHT);
        }
    }
    return bC1TouchedTheTFirst;
}

/**
 * @brief Fonction pour ajuster le robot sur le T
 * @param none
 * @return void
 * @note Utilise le troisème capteur IR et 
 * s'arrête quand C2 et C3 détecte la ligne oir
 */
void vAdjustOnT(bool bC1TouchedFirst)
{
    bool bReadyToThrow = 0;
    sSensorValues sSensorIR;
    while(bReadyToThrow == 0)
    {
        sSensorIR = oCapteurIR.sReadLineSensors();
        // Avance le robot si C1 et C2 détecte la ligne, jusqu'a ce que C1 ou C2 ne détecte plus la ligne
        if (sSensorIR.bC1IsOnTheLine && sSensorIR.bC2IsOnTheLine)
        {
            vSetMotor(MOTOR_LEFT, FORWARD, 180); // Avance moteur gauche
            vSetMotor(MOTOR_RIGHT, FORWARD, 180); // Avance moteur droit
        }
        // Cas ou le robot est orienté vers la gauche
        if (sSensorIR.bC1IsOnTheLine && (!sSensorIR.bC2IsOnTheLine))
        {
            vSetMotor(MOTOR_LEFT, FORWARD, 180); // Avance moteur droit
            vStopMotor(MOTOR_RIGHT);               // Arrêt moteur gauche
        }
        // Cas ou le robot est orienté vers la droite
        if (sSensorIR.bC2IsOnTheLine && (!sSensorIR.bC1IsOnTheLine))
        {
            vSetMotor(MOTOR_RIGHT, FORWARD, 180); // Avance moteur gauche
            vStopMotor(MOTOR_LEFT);               // Arrêt moteur droit
        }
        if ((!sSensorIR.bC1IsOnTheLine) && (!sSensorIR.bC2IsOnTheLine))
        {
            vStopMotor(MOTOR_LEFT);               // Arrêt moteur gauche
            vStopMotor(MOTOR_RIGHT);               // Arrêt moteur droit
            bReadyToThrow = 1;
        }
    }
}
/***********************************************************************************
 * @file AlgoRobotPutter.h
 * @author Thomas Eyer
 * @brief Header file for the algorithm of the robot putter: 
 *  - Search for the line (drive a circuit between 3 walls) with US sensors HC04
 *  - Detect if the robot step on the line with IR sensors QRD114
 *  - Follow the line with IR sensor QRD114
 *  - Adjust the robot on the T with IR sensor QRD114
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 **********************************************************************************/
#ifndef ALGO_ROBOTPUTTER_H
#define ALGO_ROBOTPUTTER_H

//------------------------------------------------------------------------------
// Lib packages includes, compiler define
//------------------------------------------------------------------------------
#include <Arduino.h>
#include "RobotPutterSensors.h"   
#include "MotorControl.h"      
#include <stdint.h>           
// Définition des constantes logiques
#define ROBOT_WIDTH_CM 20      // Largeur du robot en cm
#define MIN_RIGHT_DIST_CM 8    // Distance minimale droite avant rotation
#define MAX_RIGHT_DIST_CM 50   // Distance maximale droite avant rotation
#define MAX_BACK_DIST_CM 50    // Distance maximale avant arrêt
#define TURN_TIME_MS 100       // Durée de rotation en ms  
#define DEBUG 1                // Définir à 1 pour activer le mode debug
// -----------------------------------------------------------------------------
// Instanciation des objets
// -----------------------------------------------------------------------------
// Rappel au compilateur que des objets globales ont déjà été instancié dans RobotPutterSensors.cpp
extern UltrasonicSensors oCapteurUS;
extern QRD1114Sensors oCapteurIR;
// -----------------------------------------------------------------------------
// Declaration fonctions
// -----------------------------------------------------------------------------
void vSearchLine(void);
bool bStepOnLine(void);
bool vFollowLine(void);
void vAdjustOnT(bool bC1TouchedFirst);

#endif // ALGO_ROBOTPUTTER_H
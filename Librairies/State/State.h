/**
 * @file State.h
 * @author Thomas Eyer
 * @brief Contient seulement l'énumération StateOfRobot
 *                  afin de faciliter un accès global
 * @version 0.1
 * @date 2025-01-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef STATE_H
#define STATE_H

enum StateOfRobot
{
    eInit = 0,
    eSearchLine,
    eFollowLine,
    eStopAndAdjustOnT,
    eThrowBall,
    eWaitInstruction
};

extern StateOfRobot eStateOfRobot; // Déclaration globale

#endif //  STATE_H

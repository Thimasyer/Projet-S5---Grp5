/****************************************************************************
 * @file TimerConfig.cpp
 * @brief Configuration des Timer pour générer des interruptions
 * 
 ****************************************************************************/

#include <avr/interrupt.h>  // Nécessaire pour configurer les interupts
#include <avr/io.h>         // Nécessaire pour les registres des interrupts
#include <ServoControl.h>   // Librairie perso pour contrôler les servos 

ISR(TIMER2_COMPA_vect) 
{
    #if DEBUG
    Serial.println("Interruption Timer2");
    #endif
    if (eStateOfRobot == eSearchLine)
    {
        // Enregistrer les distances des capteurs ultrasons
        sSensorValues = sReadDistances();
    }
    else if (eSateOfRobot == eFollowTheLine)
    {
        // Fonctions lecture capteurs de ligne à implémenter
        // ...
        // Implémenter la détections du T en lisant les valeurs de sDistancesParois
        
    }
}

/**
 * @brief Active le Timer2
 */
void vEnableTimer2()
{
    TIMSK2 |= (1 << OCIE2A); // Activer l'interruption sur comparaison A
}

/**
 * @brief Désactive le Timer2
 */
void vDisableTimer2()
{
    TIMSK2 &= ~(1 << OCIE2A); // Désactiver l'interruption sur comparaison A
}
/****************************************************************************
 * @file TimerConfig.cpp
 * @brief Configuration des Timer pour générer des interruptions
 * 
 ****************************************************************************/

#include <avr/interrupt.h>  // Nécessaire pour configurer les interupts
#include <avr/io.h>         // Nécessaire pour les registres des interrupts
#include <ServoControl.h>   // Librairie perso pour contrôler les servos 


/**
 * @brief Initialisation du Timer2 pour générer une interruption toutes les 100 ms
 * @return void
 * @note OCR2A = F_CPU / (prescaler * f_interrupt) - 1
 */
void timer2_init()
{
    // Configuration du Timer2 pour générer une interruption toutes les 100 ms
    // Mode CTC (Clear Timer on Compare Match)
    TCCR2A = 0; // Mode normal
    // Prescaler 1024
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); 
    OCR2A = 155; // OCR0A = F_CPU / (prescaler * f_interrupt) - 1
                 // OCRA0 = 16000000 / (1024 * 10) - 1 = 155,25
    TIMSK2 = (1 << OCIE2A); // Activer l'interruption sur comparaison A
    sei(); // Activer les interruptions globales
}

/**
 * @brief Fonction d'interruption pour le Timer2, s'éxecute toutes les 100ms, 
 * pour lire les valeurs des capteurs ultrasons et des capteurs de ligne
 * @param TIMER2_COMPA_vect Vecteur d'interruption pour le Timer1
 * @return void
 */
ISR(TIMER2_COMPA_vect) 
{
    #if DEBUG
    Serial.println("Interruption Timer2");
    #endif
    if (eStateOfRobot == eSearchLine)
    {²
        // Enregistrer les distances des capteurs ultrasons
        sSensorValues = sReadDistances();
    }
    if else (eSateOfRobot == eFollowTheLine)
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
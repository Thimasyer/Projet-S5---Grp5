/*******************************************************************************
 * @file main.cpp
 * @brief Fichier Principal pour notre projet S5: robot putter
 * @author Gr5: Thomas EYER
 * 
 *******************************************************************************/
#include <Arduino.h>
// Lib perso 
#include <RobotPutterSensors.h> 
#include <ServoControl.h>   
#include <MotorControl.h>     
#include <AlgoRobotPutter.h>   

// Lib internes
#include <avr/interrupt.h>      // pour configurer interruptions
#include <avr/io.h>             // pour les noms de registres

// numéro des pins 
#define PinServoU 3            
#define PinServoContinuous 5           
#define PinServoOfGear 6       

// Définition des constantes
#define DEBUG 0                 // Définir à 1 pour activer le mode debug

// enum pour machine à état générale
enum eStateOfRobot
{
    eInit = 0,
    eSearchLine,     
    eFollowLine,     
    eStopAndAdjustOnT,
    eThrowBall,
    eWaitInstruction
}; // automatiquement initialisé à 0

// variable globale
eStateOfRobot eStateOfRobot = eInit;
sSensorValues sDistancesParois;
Servo myServoU;
Servo myServoContinuous;
Servo myServoOfGear;

struct sBLData
{
    int16_t s16AnglePlateforme;
	int16_t s16HauteurRampe;
	bool bRetire;
};


void setup() 
{
    vInitializeServo(myServoU, PinServoU);       
    vInitializeServo(myServoOfGear, PinServoOfGear);
    vInitializeServo(myServoContinuous, PinServoContinuous);
    timer2_init();                          // Initialiser le Timer2, car le 1 est utilisé par la librairie Servo
                                            // et le Timer0 est utilisé pour les fonctions delay() et millis()
}

void loop() 
{
    switch (eStateOfRobot)
    {
        case eInit:
            // Positionne les servo à leur position initiale
            vRotateByAngle(PinServoU, POS_U_HORRIZ);
            vAdjustHeight(PinServoContinuous, -5);

            // Si envoi d'une instruction BL passé au cas eSearchLine
            eStateOfRobot = eSearchLine;
        case eSearchLine:
            vEnableTimer2();
            // Code pour rechercher la ligne
            // ...
            // Si fonctions lecture des capteurs de ligne renvoi une valeur:
            eStateOfRobot = eFollowLine;
            break;
        case eFollowLine:
            // Code pour suivre la ligne
            // ...
            // Si capteur de ligne détecte le T:
            eStateOfRobot = eStopAndAdjustOnT;
            break;
        case eStopAndAdjustOnT: 
            vDisableTimer2();
            // Logique positionnement sur le T
            eStateOfRobot = eThrowBall;
            break;
        case eThrowBall:     
            // ajuster la hauteur de la plaque
            vAdjustHeight(PinServoContinuous, sBLData.s16HauteurRampe);
            vRotateByAngle(PinServoOfGear, sBLData.s16AnglePlateforme);
            // laisse la balle rouler
            vSlowSweep(PinServoU);
            // Attendre instruction BL
            //vSlowSweep(PinServoU);
            eStateOfRobot = eWaitInstruction;
            break;
        case eWaitInstruction:
            // Code pour attendre une instruction
            if (sBLData.bRetire)
            {
                eStateOfRobot = eThrowBall;
            }
            // Une fois instruction BL reçu passé au cas eThrowBall
            break;
        default:
            // Code par défaut si aucun cas n'est trouvé
            break;
    }
}

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
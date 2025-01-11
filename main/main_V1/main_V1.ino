/*******************************************************************************
 * @file main.cpp
 * @brief Fichier Principal pour notre projet S5: robot putter
 * @author Gr5: Thomas EYER
 * 
 *******************************************************************************/
#include <Arduino.h>
// Lib internes
#include <avr/interrupt.h>      // pour configurer des interruptions
#include <avr/io.h>             // pour les noms de registres
// Lib perso 
#include <RobotPutterSensors.h> 
#include <ServoControl.h>   
#include <MotorControl.h>     
#include <AlgoRobotPutter.h>
#include <BLFunctions.h>        

// Définitions constantes debug
#define DEBUG 1                // Définir à 1 pour activer le mode debug
#define TIME_MEASURE 1         // Définir à 1 pour activer le mode mesure de temps
#if TIME_MEASURE
uint16_t ui16Dummy;
uint64_t ui64Start, ui64End;
#endif

typedef double f64_t; 

// Déclaration des variables globales
StateOfRobot eStateOfRobot = eInit;
sSensorValues sSensorData;      // Structure pour stocker les valeurs des capteurs
Servo oServoU;                  // Instanciations des objets servo
Servo oServoRampe;
Servo oSeroPlateforme;
BLFunctions oBLFunctions;       // Instanciation de l'objet Bluetooth

// ------------------------ fonction setup --------------------- //
/**
 * @brief Fonctions d'initialisation Arduino
 * 
 */
void setup() 
{
    Serial.begin(9600); 
    vInitializeServo(oServoU, PinServoU);       
    vInitializeServo(oServoRampe, PinServoRampe);
    vInitializeServo(oSeroPlateforme, PinServoPlateforme);
    vMotorDriverInit(PinDir2, PinPWM2, PinDir1, PinPWM1);
    #if DEBUG
    Serial.println("Initialisations terminé, prêt.");
    #endif
    
    //timer2_init();                          // Initialiser le Timer2, car le 1 est utilisé par la librairie Servo
                                            // et le Timer0 est utilisé pour les fonctions delay() et millis()
}

// ------------------------ fonction loop --------------------- //
/**
 * @brief Fonction principale Loop d'Arduino
 * 
 */
void loop() 
{
    // Mesure du temps
    #if TIME_MEASURE
    ui64Start = millis();
    #endif

    switch (eStateOfRobot)
    {
        case eInit:
            // Positionne les servo à leur position initiale
            vRotateByAngle(oServoU, POS_U_HORRIZ);
            vAdjustHeight(oServoRampe, -5);

            // Si envoi d'une instruction BL passé au cas eSearchLine

            break;
        case eSearchLine:
            //vEnableTimer2();
            // Fonction boucle pour rechercher la ligne
            vSearchLine();
            if(bStepOnLine()) 
            {
                eStateOfRobot = eFollowLine;
            }
            else        // si erreur
            {
                vSetMotor(MOTOR_LEFT, BACKWARD, 180); // Avance moteur gauche
                vSetMotor(MOTOR_RIGHT, BACKWARD, 180); // Avance moteur droit
                delay(1000);
                vStopMotor(MOTOR_LEFT);
                vStopMotor(MOTOR_RIGHT);
                eStateOfRobot = eSearchLine;
            }
            break;
        case eFollowLine:
            // Fonction boucle pour suivre la ligne

            if(vFollowLine())       // renvoi 1 si C1 touche en premier
            {                       // revoi 0 si C1 ne touche en premier
                vAdjustOnT(1);
            }
            else 
            {
                vAdjustOnT(0);
            }
            // Si capteur de ligne détecte le T:
            eStateOfRobot = eThrowBall;
             break;
        case eThrowBall:     
            // ajuste l'angle de tir de la platforme
            if (oBLFunctions.ui16GetAdjustAngle() != 0)
            {
                vRotateByAngle(oSeroPlateforme, oBLFunctions.ui16GetAdjustAngle());
                oBLFunctions.vSetAdjustAngle(0);   // remise à zéro
            }
            // ajuster la hauteur de la plaque
            if (oBLFunctions.ui16GetAdjustHeight() != 0)
            {
                vAdjustHeight(oServoRampe, oBLFunctions.ui16GetAdjustHeight());
                oBLFunctions.vSetAdjustHeight(0);  // remise à zéro
            }
           
            // laisse la balle rouler
            vSlowSweep(oServoU);
            eStateOfRobot = eWaitInstruction;
            break;
        case eWaitInstruction:
            // Code pour attendre une instruction
            if (oBLFunctions.eGetNextState != eInit)
            {
                eStateOfRobot = oBLFunctions.eGetNextState();
                oBLFunctions.vSetNextState(eInit); // remise à zéro
            }
            break;
        default:
            // Code par défaut si aucun cas n'est trouvé
            break;
    }
    // mets à jour les données reçu en BL
    oBLFunctions.vReceiveData();

    // Mesure du temps, et affichage
    if ((ui16Dummy%5)== 0)
    {
        #if TIME_MEASURE
        // Deuxième mesure du temps
        ui64End = millis();
        // Calcul du temps écoulé
        uint32_t ui32ElapsedTime= (uint32_t)(ui64End - ui64Start);
        Serial.println("Temps d'exécution (en s): ");
        Serial.println(ui32ElapsedTime);
        #endif
    }
    ui16Dummy++;
}

// /**
//  * @brief Initialisation du Timer2 pour générer une interruption toutes les 100 ms
//  * @return void
//  * @note OCR2A = F_CPU / (prescaler * f_interrupt) - 1
//  */
// void timer2_init()
// {
//     // Configuration du Timer2 pour générer une interruption toutes les 100 ms
//     // Mode CTC (Clear Timer on Compare Match)
//     TCCR2A = 0; // Mode normal
//     // Prescaler 1024
//     TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); 
//     OCR2A = 155; // OCR0A = F_CPU / (prescaler * f_interrupt) - 1
//                  // OCRA0 = 16000000 / (1024 * 10) - 1 = 155,25
//     TIMSK2 = (1 << OCIE2A); // Activer l'interruption sur comparaison A
//     sei(); // Activer les interruptions globales
// }

// /**
//  * @brief Fonction d'interruption pour le Timer2, s'éxecute toutes les 100ms, 
//  * pour lire les valeurs des capteurs ultrasons et des capteurs de ligne
//  * @param TIMER2_COMPA_vect Vecteur d'interruption pour le Timer1
//  * @return void
//  */
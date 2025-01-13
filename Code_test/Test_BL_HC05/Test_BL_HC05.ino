

// // ========== Librairies à inclure ========== //

// // Créer un port de communication série virtuel
// #include <SoftwareSerial.h>


// // ======= Déclaration des variables ======= //

// // Port série pour communication bluetooth
// SoftwareSerial hc05(10, 11) ; // RX, TX

// void setup() {

//   // Définitions des baudrates des ports série
//   Serial.begin(9600) ;
//   hc05.begin(38400) ;

//   // Déclaration pin 9 en output et initialisation à 1
//   pinMode(9,OUTPUT) ;
//   digitalWrite(9,HIGH) ;

//   // Message d'accueil
//   Serial.println("AT commands:") ;
  
// }

// void loop() {

//   // Si un message arrive sur le bluetooth
//   if (hc05.available()){  
//     Serial.println("Available");
//     // On l'écrit sur le terminal Arduino
//     Serial.write(hc05.read());
//   }

//   // Si un message arrive sur le terminal Arduino
//   if (Serial.available())  {
//     Serial.println("Message arrivé");
//     // On l'écrit sur le bluetooth
//     hc05.write(Serial.read());
//   }
// }

#include "SoftwareSerial.h"
#include <String.h>
SoftwareSerial mySerial(2, 3); // broshes RX, TX de l'Arduino 
                                // à brancher en croisé avec le module HC05

int val;
String sBuffer;

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(38400);
  Serial.println("Entrez une commande AT:");
}

void loop() 
{
  // // Continuez à lire à partir du HC-05 et envoyez-le au moniteur série Arduino
  // if (BTSerial.available())
  // {
  //   //Serial.println("Reading BL and writing to serial");
  //   Serial.write(BTSerial.read());
  //   delay(100); 
  // }

  // // Continuez à lire depuis le moniteur série Arduino et envoyez-le au HC-05
  // if (Serial.available())
  // {
  //   //Serial.println("Reading serial and writing to BL");
  //   BTSerial.write(Serial.read());
  //   delay(100);
  // }

  while (mySerial.available())
  {
    char c = mySerial.read();
    Serial.print(c);
    if ((c == '\r')||(c == '\n')||(c == '#')) // End of message
    {
        Serial.print("La chaine complète:");
        Serial.println(sBuffer);
        sBuffer = ""; // Clear buffer
    }
    else
    {
        sBuffer += c; // Append character to buffer
        //Serial.println("En cours");
    }
  }
}

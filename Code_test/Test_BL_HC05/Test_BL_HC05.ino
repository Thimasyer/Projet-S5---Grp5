

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
SoftwareSerial mySerial(2, 3); // broshes TX, RX

int val;
String sBuffer;

void setup() {
  Serial.begin(9600);
  mySerial.begin(38400);
  pinMode(13, OUTPUT);
}

void loop() {
  if (mySerial.available()) {

    //val = mySerial.read();
    Serial.println(val);
    if (val == '1') {digitalWrite(13, HIGH);}
    if (val == '0') {digitalWrite(13, LOW);}

    while (mySerial.available())
    {
        char c = mySerial.read();
        Serial.print(c);
        if (c == '\n') // End of message
        {
            sBuffer = ""; // Clear buffer
            Serial.print("La chaine complète:");
            Serial.println(sBuffer);
        }
        else
        {
            sBuffer += c; // Append character to buffer
            //Serial.println("En cours");
        }
    }
  }
}

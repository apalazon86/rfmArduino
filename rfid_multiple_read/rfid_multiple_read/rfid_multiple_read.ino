//Se incluyen librerias para manejar el bus SPI y el lector RFID
#include <SPI.h>
#include <MFRC522.h>

//Definimos los pines esclavo y de reset para la comunicacion SPI
#define SS_PIN 53
#define RST_PIN 5
 
//Se instancia un objeto de lector MFRC522
MFRC522 rfid(SS_PIN, RST_PIN); 
//Creacion de struct key que almacenará la información de una tarjeta
MFRC522::MIFARE_Key key;

//Cadenas para almacenar el NUID

String NUID="";

void setup() { 
  //Inicializacion del puerto serie
  Serial.begin(9600);
  //Inicializacion del bus SPI
  SPI.begin();

  //Inicializacion MFRC522
  rfid.PCD_Init();
  //Se inicializan los 6 bytes  de key a su valor de fabrica 0xFF
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println("Esperando tarjetas");
}
 
void loop() {

  //En espera de que se aproxime una tarjeta al lector     
  //Si no hay una tarjeta presente, se vuelve al inicio del bucle
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  //En caso contrario se lee la tarjeta si no se pudo leer se vuelve al inicio del bucle
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  NUID="";
  
  //Se almacena la NUID
  for (byte i = 0; i < 4; i++) {
    NUID+= rfid.uid.uidByte[i];
  }  
  //Se muestra la NUID por el monitor serie
  Serial.println("Se ha leido la tarjeta con NUID: "+NUID);
  
  //Se detiene la lectura anterior
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

}

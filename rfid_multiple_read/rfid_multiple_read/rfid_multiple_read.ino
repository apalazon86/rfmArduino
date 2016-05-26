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

//Cadenas para almacenar los NUIDs ya leidos y el actual
String oldNUIDS="";
String NUID="";

//Lectura totales
int total=0;
//Lecturas simultaneas
int lecturasSimultaneas=0;

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

}
 
void loop() {

  //En espera de que se aproxime una tarjeta al lector     
  //Si no hay una tarjeta presente, se vuelve al inicio del bucle
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  //En caso contrario se lee la tarjeta si no se pudo leer se vuelve al inicio del bucle
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Se almacena la NUID
  for (byte i = 0; i < 4; i++) {
    NUID+= rfid.uid.uidByte[i];
  }
  
  //Se muestra la NUID por el monitor serie
  Serial.println(NUID);
  
  //Se le concatena una almohadilla
  NUID+="#";

  //Si la tarjeta ya ha sido leida
  if(StringContains(oldNUIDS,NUID)>=0){
    Serial.println("La tarjeta ya se ha leido");
  }
  else{
    Serial.println("Tarjeta nueva leida");
    oldNUIDS+=NUID;
    total++;
  }
  //Se detiene la lectura anterior
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  //Se inicializa a vacio NUID para almacenar la siguiente lectura
  NUID="";
  Serial.println("Lecturas totales");
  Serial.println(total);
}

//Funcion para saber si una cadena esta contenida en otra
int StringContains(String s, String search) {
    int max = s.length() - search.length();
    int lgsearch = search.length();

    for (int i = 0; i <= max; i++) {
        if (s.substring(i, i + lgsearch) == search) return i;
    }

 return -1;
}


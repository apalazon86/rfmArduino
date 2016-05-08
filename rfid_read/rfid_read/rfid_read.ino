//Se incluyen librerias para manejar el bus SPI, el lector RFID y el modulo ethernet
#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>

//Definimos los pines esclavo y de reset para la comunicacion SPI
#define SS_PIN 53 
#define RST_PIN 5  

//Asignamos una direccion MAC e IP al Arduino
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 187);

//Determinamos a que servidor se va a mandar la informacion
char server[] = "rfmapp.cloudno.de";

//Cliente ethernet
EthernetClient client;

//Sistema RFM 
const String rfm="0001";

//Ubicacion de la antena
//01-->Frigorifico
//02-->Despensa 1
//03-->Despensa 2
const String antenna="01";

//JSON que se va a enviar via ethernet

String data="";

//Se instancia un objeto de lector MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
//Creacion de struct key que almacenará la información de una tarjeta
MFRC522::MIFARE_Key key;

//Array donde se almacena que bloques van a ser leidos
int blocks[6]={62,61,60,58,57,56};

//String para almacenar la lectura de los bloques
String strBlocks="";

//Array de bytes donde se almacena la lectura de cada bloque
byte readbackblock[18];

void setup() {
  //Inicializacion del puerto serie
  Serial.begin(9600);
  
  //Inicializacion bus SPI      
  SPI.begin();
  
 //Inicializacion MFRC522 
  mfrc522.PCD_Init();
  //Se inicializan los 6 bytes  de key a su valor de fabrica 0xFF
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //Inicializacion modulo Ethernet
  
  //Se intenta configurar mediante DCHP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("No se pudo configurar mediante DCHP");
    //Si falla, se configura sin DCHP
    Ethernet.begin(mac, ip);
  }
  //Se da un segundo al modulo para que se inicie
  delay(1000);
  Serial.println("Iniciando Ethernet Shield");
  Serial.println("Esperando a leer productos");
}

void loop()
{
  //Se detiene la lectura anterior
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  //En espera de que se aproxime una tarjeta al lector     
  //Si no hay una tarjeta presente, se vuelve al inicio del bucle
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  
  //En caso contrario se lee la tarjeta si no se pudo leer se vuelve al inicio del bucle
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

    
  Serial.println("Tarjeta Detectada");

  //Se lee el contenido de los bloques 56, 57, 58, 60, 61 y 62
  //Se inicializa el valor de la lectura de los bloques
  strBlocks="";
  for (int i=0; i<6; i++){
    //Se lee un bloque
    readBlock(blocks[i], readbackblock);
    for (int j=0 ; j<16 ; j++){
      //Se convierte de decimal a hexadecimal cada uno de sus elementos y se almacena
      strBlocks=strBlocks+String(readbackblock[j], HEX);
    }
  }

  //Si se han leido los 6 bloques al completo
  if(strBlocks.length()==192){
    Serial.println("Producto leido"); 
    //Se compone el JSON que se va a postear
    data="{\"rfm\":\""+rfm+"\",\"antenna\":\""+antenna+"\",\"arduino\":\""+strBlocks+"\"}";
    Serial.println(data);
    
    //Se envia la lectura mediante el modulo ethernet

    //Se cierra la conexion anterior
    client.stop();
  /*0
    //Si se consigue conectar con el servidor
    if (client.connect(server, 80)) {
      Serial.println("conectado");
      client.println("POST /products HTTP/1.1"); 
      client.println("Host: rfmapp.cloudno.de"); 
      client.println("Content-Type: application/json"); 
      client.print("Content-Length: "); 
      client.println(data.length()); 
      client.println(); 
      client.print(data);   
      Serial.println("post realizado");
    }
    else {
      //En caso contrario se informa de que la conexion ha fallado
      Serial.println("conexion fallida");
    }*/
  //Se espera un poco a hacer la siguiente lectura
  delay(1000);
  }
  else{
    Serial.println("No se pudo leer el producto");
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funciones///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Funcion para leer un bloque, esta funcion se ha encontrado en los ejemplos del creador de la libreria MFRC522 https://github.com/miguelbalboa/rfid
int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector

  /*****************************************authentication of the desired block for access***********************************************************/
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  //byte PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
  //this method is used to authenticate a certain block for writing or reading
  //command: See enumerations above -> PICC_CMD_MF_AUTH_KEY_A  = 0x60 (=1100000),    // this command performs authentication with Key A
  //blockAddr is the number of the block from 0 to 15.
  //MIFARE_Key *key is a pointer to the MIFARE_Key struct defined above, this struct needs to be defined for each block. New cards have all A/B= FF FF FF FF FF FF
  //Uid *uid is a pointer to the UID struct that contains the user ID of the card.
  if (status != MFRC522::STATUS_OK) {
        // Serial.print("PCD_Authenticate() failed (read): ");
         return 3;//return "3" as error message
  }
  //it appears the authentication needs to be made before every block read/write within a specific sector.
  //If a different sector is being authenticated access to the previous one is lost.


  /*****************************************reading a block***********************************************************/
        
  byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          return 4;//return "4" as error message
  }
  //Serial.println("bloque leido");
}

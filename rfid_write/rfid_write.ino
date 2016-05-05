//Se incluyen librerias para manejar el bus SPI y el lector RFID
#include <SPI.h>
#include <MFRC522.h>

//Definimos los pines esclavo y de reset
#define SS_PIN 53 
#define RST_PIN 5  

//Se instancia un objeto de lector MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
//Creacion de struct key que almacenará la información de una tarjeta
MFRC522::MIFARE_Key key;

//Declaracion de variables
String productName;//Nombre de producto
int addedSpaces;//Espacios que hay que añadir al nombre del producto

//Bloques en los que se almacena el nombre de producto
String block62;
String block61;
String block60;

String block58;//Bloque 2 del sector 14, contiene foodId,group,energy,quantity
String block57;//Bloque 1 del sector 14, contiene fat,carbo,protein,cGluten,cEgg,cMilk,cSoy,cPeanuts,cNut,cFish
String block56;//Bloque 0 del sector 14, contiene cShellFish,cCellery,cSesame,CMustard,fresh,dateOfExpiry,units

String cAllergies;//Para saber si el producto tiene alergenos
String fresh;//para saber si el producto es fresco

byte blockcontent[16];//Informacion a guardar en un bloque

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
}

void loop() {
    
  //productName
  Serial.println("Introduzca el nombre del producto:");
  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  
  productName=Serial.readString(); 
  
  //Si el producto tiene menos de 48 caracteres se rellena con espacios en blanco
  if(productName.length()<48){
    //Se calcula cuantos espacios hay que añadir al nombre de producto
    addedSpaces=48-productName.length();
    for (int i=0; i <addedSpaces  ; i++){
      productName=productName+' ';
   } 
  }
  //En caso contrario se corta la cadena, dejando los 48 primeros caracteres
  else{
    productName=productName.substring(0,48);
  }
  
  //Se guarda el nombre de producto en sus respectivos bloques de memoria
  block62=productName.substring(0,16);
  block61=productName.substring(16,32);
  block60=productName.substring(32,48);

  //foodId
  block58="";
  Serial.println("Introduzca el foodId: (ddddd)");
  
  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block58=block58+leftZeros(Serial.readString(),5);

  //Se añade el grupo
  block58=block58+block58.substring(0,2);

  //energy
  Serial.println("Introduzca las kcals:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block58=block58+leftZeros(Serial.readString(),4);

  //quantity
  Serial.println("Introduzca la cantidad de producto:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block58=block58+leftZeros(Serial.readString(),5);

  block57="";
  
  //fat
  Serial.println("Introduzca los g de grasa en cada 100g:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block57=block57+leftZeros(Serial.readString(),3);

   //carbo
  Serial.println("Introduzca los g de carbohidratos en cada 100g:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block57=block57+leftZeros(Serial.readString(),3);

  //protein
  Serial.println("Introduzca los g de proteina en cada 100g:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block57=block57+leftZeros(Serial.readString(),3);

  //cAllergies
  Serial.println("¿El producto contiene alergenos? Y/N");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  
  cAllergies=Serial.readString();

  //Si no tiene alergenos se inicializan a falso
  if(cAllergies=="N"){
    block57=block57+"NNNNNNN";
    block56="NNNN";
  }
  else{
    //cGluten
    Serial.println("¿El producto contiene gluten? Y/N");

    //Wait for user input
    while (Serial.available()==0) {             

    }
    block57=block57+Serial.readString();

    //cEgg
    Serial.println("¿El producto contiene huevo? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();
  
    //cMilk
    Serial.println("¿El producto contiene lacteos? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();
  
    //cSoy
    Serial.println("¿El producto contiene soja? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();
  
    //cPeanuts
    Serial.println("¿El producto contiene cacahuetes? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();

    //cNut
    Serial.println("¿El producto contiene frutos con cascara? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();

    //cFish
    Serial.println("¿El producto contiene pescado? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block57=block57+Serial.readString();

    //cShellFish
    Serial.println("¿El producto contiene marisco? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block56=Serial.readString();

    //cCellery
    Serial.println("¿El producto contiene apio? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block56=block56+Serial.readString();

    //cSesame
    Serial.println("¿El producto contiene sesamo? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block56=block56+Serial.readString();

    //cMustard
    Serial.println("¿El producto contiene mostaza? Y/N");
  
    //Wait for user input
    while (Serial.available()==0) {             
    
    }
    block56=block56+Serial.readString();
      
  }

  //fresh
  Serial.println("¿El producto es fresco? Y/N");
    
  //Wait for user input
  while (Serial.available()==0) {             
      
  }
  fresh=Serial.readString();
  block56=block56+fresh;

  //dateOfExpiry

  if(fresh=="N"){
    Serial.println("Introduzca el dia de la fecha de caducidad (DD)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+leftZeros(Serial.readString(),2);

    Serial.println("Introduzca el mes de la fecha de caducidad (MM)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+leftZeros(Serial.readString(),2);

    Serial.println("Introduzca el año de la fecha de caducidad (YYYY)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+Serial.readString();
  }
  else{
    Serial.println("Introduzca el dia de la fecha de envasado (DD)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+leftZeros(Serial.readString(),2);

    Serial.println("Introduzca el mes de la fecha de envasado (MM)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+leftZeros(Serial.readString(),2);

    Serial.println("Introduzca el año de la fecha de envasado (YYYY)");
    //Wait for user input
    while (Serial.available()==0) {             
      
    }
    block56=block56+Serial.readString();


  }
  //Units
  Serial.println("Introduzca las unidades en las que se mide la cantidad de producto:");

  //Wait for user input
  while (Serial.available()==0) {             
  
  }
  block56=block56+rightBlank(Serial.readString(),4);
  
  Serial.println("//////////////////////////////////////////////////////////////////////////");
  Serial.println("Se va a escribir la tarjeta con la informacion que se acaba de introducir");
  Serial.println("Aproxime una tarjeta");
  //En espera de que se aproxime una tarjeta al lector     

  //Si no hay una tarjeta presente, se vuelve al inicio del bucle
  while(! mfrc522.PICC_IsNewCardPresent()){
  
  }
  
  //En caso contrario se lee la tarjeta si no se pudo leer se vuelve al inicio del bucle
  while(! mfrc522.PICC_ReadCardSerial()){
  
  }

  Serial.println("Tarjeta Detectada");
  
  //Escritura de la tarjeta

  //Guardamos el contenido de block62 en blockcontent
  fillBlock(blockcontent,block62);
  //Escribimos el contenido de blockcontent en el bloque 62 de nuestra tarjeta
  writeBlock(62, blockcontent);

  //Se hace lo mismo con el resto de bloques
  fillBlock(blockcontent,block61);
  writeBlock(61, blockcontent);
  
  fillBlock(blockcontent,block60);
  writeBlock(60, blockcontent);

  fillBlock(blockcontent,block58);
  writeBlock(58, blockcontent);

  fillBlock(blockcontent,block57);
  writeBlock(57, blockcontent);

  fillBlock(blockcontent,block56);
  writeBlock(56, blockcontent);
  
}

//Funcion para añadir ceros a la izquierda
String leftZeros(String num, int zeros){
  while (num.length()<zeros){
    num='0'+num;
  }
return num;
}

//Funcion para añadir espacios a la derecha
String rightBlank(String str, int blanks){
  while (str.length()<blanks){
    str=str+' ';
  }
return str;
}

//Funcion para rellenar un bloque
void fillBlock(byte blockcontent[], String str){
  for (int i=0; i <16  ; i++){
    blockcontent[i]=str[i];
   } 
}

///////////////////////////////////////////////////////////////
//Funciones empleadas para la lectura y escritura de bloques
//////////////////////////////////////////////////////////////
//El codigo se ha sacado de este tutorial http://makecourse.weebly.com/week10segment1.html

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}//block number is a trailer block (modulo 4); quit and send error code 2
  Serial.print(blockNumber);
  Serial.println(" is a data block:");
  
  /*****************************************authentication of the desired block for access***********************************************************/
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  //byte PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
  //this method is used to authenticate a certain block for writing or reading
  //command: See enumerations above -> PICC_CMD_MF_AUTH_KEY_A  = 0x60 (=1100000),    // this command performs authentication with Key A
  //blockAddr is the number of the block from 0 to 15.
  //MIFARE_Key *key is a pointer to the MIFARE_Key struct defined above, this struct needs to be defined for each block. New cards have all A/B= FF FF FF FF FF FF
  //Uid *uid is a pointer to the UID struct that contains the user ID of the card.
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
        // Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  //it appears the authentication needs to be made before every block read/write within a specific sector.
  //If a different sector is being authenticated access to the previous one is lost.


  /*****************************************writing the block***********************************************************/
        
  status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);//valueBlockA is the block number, MIFARE_Write(block number (0-15), byte array containing 16 values, number of bytes in block (=16))
  //status = mfrc522.MIFARE_Write(9, value1Block, 16);
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           //Serial.println(mfrc522.GetStatusCodeName(status));
           return 4;//return "4" as error message
  }
  Serial.println("bloque escrito");
}

#include <SPI.h>//include the SPI bus library
#include <MFRC522.h>//include the RFID reader library

#define SS_PIN 53  //slave select pin
#define RST_PIN 5  //reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN);        // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key;//create a MIFARE_Key struct named 'key', which will hold the card information

int blocks[6]={62,61,60,58,57,56};

String strBlocks="";
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
}

void loop()
{

  strBlocks="";
  // Look for new cards (in case you wonder what PICC means: proximity integrated circuit card)
	if ( ! mfrc522.PICC_IsNewCardPresent()) {//if PICC_IsNewCardPresent returns 1, a new card has been found and we continue
		return;//if it did not find a new card is returns a '0' and we return to the start of the loop
	}
	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {//if PICC_ReadCardSerial returns 1, the "uid" struct (see MFRC522.h lines 238-45)) contains the ID of the read card.
		return;//if it returns a '0' something went wrong and we return to the start of the loop
	}
 
  Serial.println("card selected");

  for (int i=0; i<6; i++){
    readBlock(blocks[i], readbackblock);//read the block back
    //print the block contents
    for (int j=0 ; j<16 ; j++){
      strBlocks=strBlocks+String(readbackblock[j], HEX);
    }
  }
  
  Serial.println("Producto leido");
  Serial.println(strBlocks);
 
  // Halt PICC
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(500);
}


Funciones
//////////////////////////////////////////////////////////////////////////////////////////////////////////
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
         Serial.print("PCD_Authenticate() failed (read): ");
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
  //Serial.println("block was read");
}

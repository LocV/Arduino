/*
 * 
 * Created by Loc Vuu
 * 
 * February 2017
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#define RED_FOB "14 5E DC EC"
#define BLU_FOB "A4 86 5A EB"
#define MASTER "D3 35 F3 75"

#define RELAY_PIN 8   // Connect Digital Pin 8 on Arduino to CH1 on Relay Module

// Initiate Buzzer
const int buzzerPin = 7;
const int duration = 500;
const int accessGrantedHz = 800;
const int accessDeniedHz = 260;

void setup() 
{
  // Initiate Buzzer
  pinMode(buzzerPin, OUTPUT);

  // Initiate Relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // turn the relay off

  // Initiate RFID RCC-522
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase(); 
  
  if ((content.substring(1) == BLU_FOB) ||
      (content.substring(1) == RED_FOB) ||
      (content.substring(1) == MASTER))
    {
      // Card ID is allowed access
      Serial.println("Authorized access");
      Serial.println();
      playTone(accessGrantedHz); // Play tone for access Granted
      activateSolenoid();
      delay(2000);
          
    } else{
      // card ID not in allowed list
      Serial.println(" Access denied");
      playTone(accessDeniedHz); // Play tone for invalid access attempt
      delay(3000);
   }
}

void playTone(int frequency){
  tone(buzzerPin, frequency, duration);
}

void activateSolenoid(){
  int timeDelay = 2000;
  
  digitalWrite(RELAY_PIN, HIGH);  // turn the relay on
  delay(timeDelay);              // wait for one second
  digitalWrite(RELAY_PIN, LOW);   // turn the relay off
  delay(timeDelay);       
}




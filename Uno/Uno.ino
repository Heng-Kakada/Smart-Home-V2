#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>



#define SS_PIN 10
#define RST_PIN 9
#define BUTTON_PIN 2
#define RFID_PATH "rfid"
#define BUZZER_PIN 3

// number of items in an array
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String keys[] = {
  "83 4A 14 0E"
};

int cardAttempt = 0;
const int DEBOUNCE_DELAY = 50;   // the debounce time; increase if the output flickers
unsigned long lastDebounceTime = 0;
int lastSteadyState = LOW;
int lastFlickerableState = LOW;

bool arrayContainsString(String target) {
    for (int i = 0; i < NUMITEMS(keys); i++) {
        if (target.equals(keys[i])) {
            return true;  // Found the string in the array
        }
    }
    return false;  // String not found in the array
}

void transmit(int data, const char *name) {
      JsonDocument doc;

      doc["path"] = name;
      doc["data"] = data;

      serializeJson(doc, Serial);
      Serial.println();
}

void buzzerAlert(){
    for (int i = 0; i < 100; i++) {  // make a sound
    digitalWrite(BUZZER_PIN, HIGH); // send high signal to buzzer 
    delay(1); // delay 1ms
    digitalWrite(BUZZER_PIN, LOW); // send low signal to buzzer
    delay(1);
  }
  delay(50);
}

void readButton(){
  byte buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastFlickerableState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastFlickerableState = buttonState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (lastSteadyState == HIGH && buttonState == LOW){
      Serial.println("The button is pressed");
      transmit(0, RFID_PATH);
    }
    // save the the last steady state
    lastSteadyState = buttonState;
  }
}

void readRFID(){
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

  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
//  if (content.substring(1) == "83 4A 14 0E") //change here the UID of the card/cards that you want to give access
  if(arrayContainsString(content.substring(1)))
  {
    Serial.println("Authorized access");
    Serial.println();
    transmit(1, RFID_PATH);
    cardAttempt = 0;
    delay(3000);
  }
  else{
    cardAttempt += 1;
    Serial.println(cardAttempt);
    buzzerAlert();
    if(cardAttempt == 3)
    {
      Serial.println("Maximum attempted reached");
      transmit(-1, RFID_PATH);
      cardAttempt = 0;
      for(int i = 0; i < 5; i++){
        buzzerAlert();
      }
      delay(3000);
    }
    delay(3000);
  }
}


void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Init Button Pin
  pinMode(BUZZER_PIN, OUTPUT); // Init Buzzer Pin
  Serial.println("Initialization");
}

void loop() {
  readButton();
  readRFID();

}

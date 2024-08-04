  #include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <Wire.h>

// My Library

#include "Globals.h"

void keyPress(){

  if (lockout) {
    if (millis() - lockoutTime >= lockoutDuration) {
      lockout = false;
      attemptCounter = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Locked for ");
      lcd.print((lockoutDuration - (millis() - lockoutTime)) / 1000);
      lcd.print(" sec");
      return;
    }
  }
  
  char key = keypad.getKey();
  
  if(key){
      if (key == '#') {
        lcd.clear();
        if (inputPassword == PASSWORD) {
          
          lcd.setCursor(0,1);
          lcd.print("Access Granted");

          JsonDocument doc;

          doc["path"] = "frontDoor";
          doc["data"] = 1;

          serializeJson(doc, Serial);
          Serial.println();

          scFrontDoor.activate();

          // open door 
          doorUnlocked = true;
          unlockTime = millis();
          attemptCounter = 0; // Reset the attempt counter on successful entry
          
        } else {
          lcd.setCursor(0,1);
          lcd.print("Access denied");
          attemptCounter++;
          if (attemptCounter >= maxAttempts) {
            lockout = true;
            lockoutTime = millis();
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("System Locked");

            JsonDocument doc;
            doc["path"] = "frontDoor";
            doc["data"] = -1;

            serializeJson(doc, Serial);
            Serial.println();
          } else {
            unlockTime = millis();
          }
        }
        inputPassword = ""; // Clear the input
        if (!lockout) {
          lcd.setCursor(0, 0);
          lcd.print("Enter Password:");
        }
    } else if (key == '*') {
      inputPassword = ""; // Clear the input if * is pressed
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
    } else {
      inputPassword += key; // Append the key to the input password
      lcd.setCursor(0, 1);
      lcd.print(inputPassword); // Display the input
    }
  }
  
  if(doorUnlocked && millis() - unlockTime >= unlockDuration){
    // Lock door again
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    
    doorUnlocked = false;
    
    JsonDocument doc;
    doc["path"] = "frontDoor";
    doc["data"] = 0;

    serializeJson(doc, Serial);
    Serial.println();

    scFrontDoor.deactivate();
  }
}

void dhtFunction(){
  if (currentTime - dhtPrevMillis >= dhtPeriod)
  {
    dhtPrevMillis = currentTime;
    dthSensor.transmit();
  }
}

void readSerial(){ 
    String incomingData = "";
    DynamicJsonDocument doc(1024);
    if (Serial.available() > 0) {
        incomingData = Serial.readString();
        DeserializationError err = deserializeJson(doc, incomingData);
        if (err) {
          return;
        }
        const char* root = doc["root"];
        const char* path = doc["path"];
        int data = doc["data"];

        if(strcmp(root, LIGHT_ROOT) == 0){
          lightAction(path, data);
        }else if(strcmp(root, ACTUATORS_ROOT) == 0){
          actuatorAction(path, data);
        }else if(strcmp(root, MODE_ROOT) == 0){
          modeAction(data);
        }
    }
}

void lightAction(const char *path,int data){
  if(strcmp(path,L_LIVINGROOM_PATH) == 0){
    lLivingRoom.setState(data);
  }else if(strcmp(path,L_KITCHEN_PATH) == 0){
    lKitchen.setState(data);
  }else if(strcmp(path,L_GARAGE_PATH) == 0){
    lGarage.setState(data);
  }else if(strcmp(path,L_BEDROOM1_PATH) == 0){
    lBedRoom1.setState(data);
  }else if(strcmp(path,L_BEDROOM2_PATH) == 0){
    lBedRoom2.setState(data);
  }else if(strcmp(path,L_FIRSTFLOOR_PATH) == 0){
    lFirstFloor.setState(data);
  }else if(strcmp(path,L_BACONY_PATH) == 0){
    lBacony.setState(data);
  }else if(strcmp(path,L_GARAGEFRONT_PATH) == 0){
    lFrontGarage.setState(data);
  }else if(strcmp(path,L_FRONTDOOR_PATH) == 0){
    lFrontDoor.setState(data);
  }
}

void curtainAction(int data){
  // curtain
  if(data == 1){
    curtain.forward(60);
    delay(curtain.open_duration);
    curtain.stop();
  }else{
    curtain.backward(80);
    delay(curtain.close_duration);
    curtain.stop();
  }
}

void garageDoorAction(int data){
  // garage door
  if(data == 1){
    doorGarage.forward(140);
    delay(doorGarage.open_duration);
    doorGarage.stop();
  }else{
    doorGarage.backward(90);
    delay(doorGarage.close_duration);
    doorGarage.stop();
  }
}

void actuatorAction(const char *path,int data){
  if(strcmp(path, A_DOOR_FRONTDOOR_PATH) == 0){
    scFrontDoor.setState(data);
  }else if(strcmp(path, A_DOOR_BEDROOM1_PATH) == 0){
    scBedRoom1.setState(data);
  }else if(strcmp(path, A_DOOR_BEDROOM2_PATH) == 0){
    scBedRoom2.setState(data);
  }else if(strcmp(path, A_FAN_KITCHEN_PATH) == 0){
    fanKitchen.setState(data);
  }else if(strcmp(path, A_CURTAIN_PATH) == 0){
    curtainAction(data);
  }else if(strcmp(path, A_DOOR_GARAGE_PATH) == 0){
    garageDoorAction(data);
  }else if(strcmp(path, A_FAN_ROOM1_PATH) == 0){
    fanBed1.setState(data);
  }else if(strcmp(path, A_FAN_ROOM2_PATH) == 0){
    fanBed2.setState(data);
  }else if(strcmp(path, A_DOOR_BALCONY_PATH) == 0){
    scBalcony.setState(data);
  }else if(strcmp(path, A_WINDOW_PATH) == 0){
    scWindow.setState(data);
  }
}

void modeAction(int data){
  if(data == 0){
    sleepMode();
  }else if(data == 1){
    dayMode();
  }else if (data == 2){
    nightMode();
  }
}

void sleepMode(){

  lLivingRoom.setState(0);
  lKitchen.setState(0);
  lGarage.setState(0);
  lFirstFloor.setState(0);
  lBedRoom1.setState(0);
  lBedRoom2.setState(0);

  lBacony.setState(0);
  lFrontGarage.setState(0);
  lFrontDoor.setState(0);

  scFrontDoor.setState(0);
  scBedRoom1.setState(0);
  scBedRoom2.setState(0);
  fanKitchen.setState(0);

  curtainAction(0);
  garageDoorAction(0);

  fanBed1.setState(1);
  fanBed2.setState(1);

  scBalcony.setState(0);
  scWindow.setState(0);


  JsonDocument doc;

  doc["path"] = "mode";

  JsonObject data = doc.createNestedObject("data");

  JsonObject light = data.createNestedObject("light");
  JsonObject actuator = data.createNestedObject("actuator");

  light[ lightName[0] ] = 0;
  light[ lightName[1] ] = 0;
  light[ lightName[2] ] = 0;
  light[ lightName[3] ] = 0;
  light[ lightName[4] ] = 0;
  light[ lightName[5] ] = 0;
  light[ lightName[6] ] = 0;
  light[ lightName[7] ] = 0;
  light[ lightName[8] ] = 0;

  actuator[ actuatorName[0] ] = 0;
  actuator[ actuatorName[1] ] = 0;
  actuator[ actuatorName[2] ] = 0;
  actuator[ actuatorName[3] ] = 0;
  actuator[ actuatorName[4] ] = 1;
  actuator[ actuatorName[5] ] = 1;
  actuator[ actuatorName[6] ] = 0;
  actuator[ actuatorName[7] ] = 0;
  actuator[ actuatorName[8] ] = 0;
  actuator[ actuatorName[9] ] = 0;


  serializeJson(doc, Serial);
  Serial.println();

}

void dayMode(){
  lLivingRoom.setState(0);
  lKitchen.setState(0);
  lGarage.setState(0);
  lFirstFloor.setState(0);
  lBedRoom1.setState(0);
  lBedRoom2.setState(0);

  lBacony.setState(0);
  lFrontGarage.setState(0);
  lFrontDoor.setState(0);

  scFrontDoor.setState(0);
  scBedRoom1.setState(0);
  scBedRoom2.setState(0);
  fanKitchen.setState(1);

  curtainAction(1);
  garageDoorAction(0);

  fanBed1.setState(0);
  fanBed2.setState(0);

  scBalcony.setState(0);
  scWindow.setState(1);

  JsonDocument doc;

  doc["path"] = "mode";

  JsonObject data = doc.createNestedObject("data");

  JsonObject light = data.createNestedObject("light");
  JsonObject actuator = data.createNestedObject("actuator");

  light[ lightName[0] ] = 0;
  light[ lightName[1] ] = 0;
  light[ lightName[2] ] = 0;
  light[ lightName[3] ] = 0;
  light[ lightName[4] ] = 0;
  light[ lightName[5] ] = 0;
  light[ lightName[6] ] = 0;
  light[ lightName[7] ] = 0;
  light[ lightName[8] ] = 0;

  actuator[ actuatorName[0] ] = 0;
  actuator[ actuatorName[1] ] = 0;
  actuator[ actuatorName[2] ] = 0;
  actuator[ actuatorName[3] ] = 1;
  actuator[ actuatorName[4] ] = 0;
  actuator[ actuatorName[5] ] = 0;
  actuator[ actuatorName[6] ] = 0;
  actuator[ actuatorName[7] ] = 0;
  actuator[ actuatorName[8] ] = 0;
  actuator[ actuatorName[9] ] = 1;

  serializeJson(doc, Serial);
  Serial.println();

}

void nightMode(){
  lLivingRoom.setState(1);
  lKitchen.setState(1);
  lGarage.setState(1);
  lFirstFloor.setState(1);
  lBedRoom1.setState(1);
  lBedRoom2.setState(1);

  lBacony.setState(1);
  lFrontGarage.setState(1);
  lFrontDoor.setState(1);

  scFrontDoor.setState(0);
  scBedRoom1.setState(0);
  scBedRoom2.setState(0);
  fanKitchen.setState(0);

  curtainAction(0);
  garageDoorAction(0);

  fanBed1.setState(1);
  fanBed2.setState(1);

  scBalcony.setState(0);
  scWindow.setState(0);

  JsonDocument doc;

  doc["path"] = "mode";

  JsonObject data = doc.createNestedObject("data");

  JsonObject light = data.createNestedObject("light");
  JsonObject actuator = data.createNestedObject("actuator");

  light[ lightName[0] ] = 1;
  light[ lightName[1] ] = 1;
  light[ lightName[2] ] = 1;
  light[ lightName[3] ] = 1;
  light[ lightName[4] ] = 1;
  light[ lightName[5] ] = 1;
  light[ lightName[6] ] = 1;
  light[ lightName[7] ] = 1;
  light[ lightName[8] ] = 1;

  actuator[ actuatorName[0] ] = 0;
  actuator[ actuatorName[1] ] = 0;
  actuator[ actuatorName[2] ] = 0;
  actuator[ actuatorName[3] ] = 0;
  actuator[ actuatorName[4] ] = 1;
  actuator[ actuatorName[5] ] = 1;
  actuator[ actuatorName[6] ] = 0;
  actuator[ actuatorName[7] ] = 0;
  actuator[ actuatorName[8] ] = 0;
  actuator[ actuatorName[9] ] = 0;


  serializeJson(doc, Serial);
  Serial.println();

}

void emergency(){
  scFrontDoor.setState(1);
  scBedRoom1.setState(1);
  scBedRoom2.setState(1);
  fanKitchen.setState(1);

  fanBed1.setState(0);
  fanBed2.setState(0);

  curtainAction(1);
  garageDoorAction(1);
  scBalcony.setState(1);
  scWindow.setState(1);

  JsonDocument doc;

  doc["path"] = "emergency";

  JsonObject data = doc.createNestedObject("data");

  data[ actuatorName[0] ] = 1;
  data[ actuatorName[1] ] = 1;
  data[ actuatorName[2] ] = 1;
  data[ actuatorName[3] ] = 1;
  data[ actuatorName[4] ] = 0;
  data[ actuatorName[5] ] = 0;
  data[ actuatorName[6] ] = 1;
  data[ actuatorName[7] ] = 1;
  data[ actuatorName[8] ] = 1;
  data[ actuatorName[9] ] = 1;

  serializeJson(doc, Serial);
  Serial.println();
}

void raining(){
  scFrontDoor.setState(0);
  scBedRoom1.setState(0);
  scBedRoom2.setState(0);
  fanKitchen.setState(0);
  fanBed1.setState(0);
  fanBed2.setState(0);
  garageDoorAction(0);
  scBalcony.setState(0);
  scWindow.setState(0);
  curtainAction(0);

  JsonDocument doc;

  doc["path"] = "raining";

  JsonObject data = doc.createNestedObject("data");

  data[ actuatorName[0] ] = 0;
  data[ actuatorName[1] ] = 0;
  data[ actuatorName[2] ] = 0;
  data[ actuatorName[3] ] = 0;
  data[ actuatorName[4] ] = 0;
  data[ actuatorName[5] ] = 0;
  data[ actuatorName[6] ] = 0;
  data[ actuatorName[7] ] = 0;
  data[ actuatorName[8] ] = 0;
  data[ actuatorName[9] ] = 0;

  serializeJson(doc, Serial);
  Serial.println();
}

void readFlame(){
  if(currentTime - flamePrevMillis >= flamePeriod){
    flamePrevMillis = currentTime;
    flameSensor.update("flame", 500, "less");
    if(flameSensor.getData() == 1){
      emergency();
    }
  }
}

void readGas(){
  if(currentTime - gasPrevMillis >= gasPeriod){
    gasPrevMillis = currentTime;
    gasSensor.update("gas", 400, "greater");
    if(gasSensor.getData() == 1){
      emergency();
    }
  }
}

void readRain(){
  if(currentTime - rainPrevMillis >= rainPeriod){
    rainPrevMillis = currentTime;
    rainSensor.update("rain", 650, "less");
    if(rainSensor.getData() == 1){
      raining();
    }

  }
}

void readIRSensor1(){
  if( currentTime - irPrevMillis1 >= irPeriod1){
    irPrevMillis1 = currentTime;
    irSensor1.update("car1");
  }
}

void readIRSensor2(){
  if( currentTime - irPrevMillis2 >= irPeriod2){
    irPrevMillis2 = currentTime;
    irSensor2.update("car2");
  }
}

void setup() {
  // light
  lBacony.begin();
  lBedRoom1.begin();
  lBedRoom2.begin();
  lFirstFloor.begin();
  lGarage.begin();
  lKitchen.begin();
  lLivingRoom.begin();
  lFrontDoor.begin();
  lFrontGarage.begin();

  // fan
  fanBed1.begin();
  fanBed2.begin();
  fanKitchen.begin();

  // set light off
  lBacony.setState(0);
  lBedRoom1.setState(0);
  lBedRoom2.setState(0);
  lFirstFloor.setState(0);
  lGarage.setState(0);
  lKitchen.setState(0);
  lLivingRoom.setState(0);
  lFrontGarage.setState(0);
  lFrontDoor.setState(0);

  // set fan off
  fanBed1.setState(0);
  fanBed2.setState(0);
  fanKitchen.setState(0);

  // ServoControl
  scBalcony.begin();
  scFrontDoor.begin();
  scBedRoom1.begin();
  scBedRoom2.begin();
  scWindow.begin();

  // flame
  flameSensor.begin();

  // Gas
  gasSensor.begin();

  // Rain
  rainSensor.begin();

  // IR
  irSensor1.begin();
  irSensor2.begin();

  // motor
  curtain.begin();
  doorGarage.begin();

  Serial.begin(9600);
  dthSensor.begin();

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");

}

void loop() {
  currentTime = millis();
  keyPress();
  dhtFunction();
  readFlame();
  readGas();
  readRain();
  readIRSensor1();
  readIRSensor2();
  readSerial();
}

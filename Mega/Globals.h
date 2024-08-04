#ifndef GLOBALS_H
#define GLOBALS_H

#include "Relay.h"
#include "DhtSensor.h"
#include "Sensor.h"
#include "IRSensor.h"
#include "ServoControl.h"
#include "Motor.h"

#pragma region [ PINS ]

#define R1_PIN 13
#define R2_PIN 12
#define R3_PIN 11
#define R4_PIN 10

#define C1_PIN 9
#define C2_PIN 8
#define C3_PIN 7
#define C4_PIN 6

// from relay

#define L_BALCONY_PIN 22
#define L_BEDROOM1_PIN 23
#define L_BEDROOM2_PIN 4
#define L_FIRSTFLOOR_PIN 25
#define L_GARAGE_PIN 26
#define L_KITCHEN_PIN 27
#define L_LIVINGROOM_PIN 28
#define L_FRONTDOOR_PIN 29
#define L_FRONTGARAGE_PIN 30

#define FAN_BEDROOM1_PIN 31
#define FAN_BEDROOM2_PIN 32
#define FAN_KITCHEN_PIN 33


// from motor driver
#define G_DOOR_IN1 42
#define G_DOOR_IN2 43
#define G_DOOR_ENA 2


#define CURTAIN_IN3 44
#define CURTAIN_IN4 45
#define CURTAIN_ENB 3


// from servor
#define WINDOW_PIN 5
#define DOOR_BALCONY_PIN 35
#define DOOR_FRONDOOR_PIN 36

#define DOOR_BEDROOM2_PIN 38
#define DOOR_BEDROOM1_PIN 46


// from sensor
#define IR_PIN1 48
#define IR_PIN2 49
#define DHT_PIN 50


#define FLAME_PIN A3
#define GAS_PIN A4
#define RAIN_PIN A5

#pragma endregion

#pragma region [ State ]

int ledState = LOW;

#pragma endregion

#pragma region [ Current Time ]

unsigned long currentTime = 0;

#pragma endregion

#pragma region [ Previous Time ]


unsigned long dhtPrevMillis = 0;
unsigned long flamePrevMillis = 0;
unsigned long gasPrevMillis = 0;
unsigned long rainPrevMillis = 0;
unsigned long irPrevMillis1 = 0;
unsigned long irPrevMillis2 = 0;


#pragma endregion

#pragma region [ Period ]

unsigned long dhtPeriod = 8000;
unsigned long flamePeriod = 2000;
unsigned long gasPeriod = 2000;
unsigned long rainPeriod = 2000;
unsigned long irPeriod1 = 2000;
unsigned long irPeriod2 = 2000;

#pragma endregion

#pragma region [ DHT Pin ]

#define DHTTYPE DHT22
DhtSensor dthSensor(DHT_PIN, DHT22);

#pragma endregion

#pragma region [ KeyPad Pin ]

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {R1_PIN, R2_PIN, R3_PIN, R4_PIN};
byte colPins[COLS] = {C1_PIN, C2_PIN, C3_PIN, C4_PIN};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String PASSWORD = "1234";
String inputPassword;

bool doorUnlocked = false;
unsigned long unlockTime = 0;
const unsigned long unlockDuration = 5000; // 5 seconds


int attemptCounter = 0;
const int maxAttempts = 3;
bool lockout = false;
unsigned long lockoutTime = 0;
const unsigned long lockoutDuration = 10000; // 10 seconds


#pragma endregion

#pragma region [ Constant name for deserialiazation from python ]


const char* LIGHT_ROOT = "light";
const char* ACTUATORS_ROOT  = "actuators";
const char* MODE_ROOT  = "mode";


const char* L_BACONY_PATH  = "balcony";
const char* L_LIVINGROOM_PATH  = "livingRoom";
const char* L_BEDROOM1_PATH  = "bedRoom1";
const char* L_BEDROOM2_PATH  = "bedRoom2";
const char* L_FIRSTFLOOR_PATH  = "firstFloor";
const char* L_GARAGE_PATH  = "garage";
const char* L_KITCHEN_PATH  = "kitchen";
const char* L_GARAGEFRONT_PATH  = "garageFront";
const char* L_FRONTDOOR_PATH  = "frontDoor";


const char* A_CURTAIN_PATH = "curtain";
const char* A_DOOR_BALCONY_PATH = "door/balcony";
const char* A_DOOR_FRONTDOOR_PATH = "door/frontDoor";
const char* A_DOOR_BEDROOM1_PATH = "door/bedRoom1";
const char* A_DOOR_BEDROOM2_PATH = "door/bedRoom2";
const char* A_DOOR_GARAGE_PATH = "door/garage";
const char* A_FAN_ROOM1_PATH = "fan/bedRoom1";
const char* A_FAN_ROOM2_PATH = "fan/bedRoom2";
const char* A_FAN_KITCHEN_PATH = "fan/kitchen";
const char* A_WINDOW_PATH = "window";



#pragma endregion

#pragma region [ Relay ]

Relay lBacony(L_BALCONY_PIN);
Relay lBedRoom1(L_BEDROOM1_PIN);
Relay lBedRoom2(L_BEDROOM2_PIN);
Relay lFirstFloor(L_FIRSTFLOOR_PIN);
Relay lGarage(L_GARAGE_PIN);
Relay lKitchen(L_KITCHEN_PIN);
Relay lLivingRoom(L_LIVINGROOM_PIN);
Relay lFrontDoor(L_FRONTDOOR_PIN);
Relay lFrontGarage(L_FRONTGARAGE_PIN);


Relay fanBed1(FAN_BEDROOM1_PIN);
Relay fanBed2(FAN_BEDROOM2_PIN);
Relay fanKitchen(FAN_KITCHEN_PIN);

#pragma endregion

#pragma region [ Servo ]

ServoControl scBalcony(DOOR_BALCONY_PIN, 40, 75);
ServoControl scFrontDoor(DOOR_FRONDOOR_PIN, 90, 170);
ServoControl scBedRoom1(DOOR_BEDROOM1_PIN, 0, 90);
ServoControl scBedRoom2(DOOR_BEDROOM2_PIN, 0, 90);
ServoControl scWindow(WINDOW_PIN, 0, 95);

#pragma endregion


#pragma region [ Sensor Init ]

Sensor flameSensor(FLAME_PIN);
Sensor gasSensor(GAS_PIN);
Sensor rainSensor(RAIN_PIN);

#pragma endregion

#pragma region [ Ir Init ]

IRSensor irSensor1(IR_PIN1);
IRSensor irSensor2(IR_PIN2);

#pragma endregion

#pragma region [ motor ]

Motor curtain(CURTAIN_ENB, CURTAIN_IN3, CURTAIN_IN4, 2000, 1500);
Motor doorGarage(G_DOOR_ENA, G_DOOR_IN1, G_DOOR_IN2, 150, 150);

#pragma endregion

LiquidCrystal_I2C lcd(0x27, 20, 4);


String lightName[] = {
  "livingRoom",
  "kitchen",
  "garage",
  "fristFloor",
  "bedRoom1",
  "bedRoom2",
  "balcony",
  "frontGarage",
  "frontDoor"
};

String actuatorName[] = {
  "frontDoor",
  "BedRoom1Door",
  "BedRoom2Door",
  "fanKitchen",
  "fanBed1",
  "fanBed2",
  "curtain",
  "garageDoor",
  "balconyDoor",
  "window"
};

#endif

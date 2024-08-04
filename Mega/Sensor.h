#include "Arduino.h"
#include "HardwareSerial.h"

#ifndef SENSOR_H
#define SENSOR_H

#include "Sensor.h"

class Sensor {
private:
    int pin;
    int sensorLevel;
    int previousState = -1;
    int currentState;
public:
    Sensor(int p) : pin(p), sensorLevel(0) {}

    void begin() {
        pinMode(pin, INPUT);
    }

    void update(const char *name,int level,const char *condition) {
        sensorLevel = analogRead(pin);

        if(strcmp(condition, "less") == 0){
          currentState = (sensorLevel < level) ? 1 : 0;
        }else if(strcmp(condition, "greater") == 0){
          currentState = (sensorLevel > level) ? 1 : 0;
        }
        
        if (currentState != previousState) {
          if (currentState == 1) {
            transmit(currentState, name);
          } else if (currentState == 0) {
            transmit(currentState, name);
          }
          previousState = currentState;
        }

    }

    int getData(){
      return currentState;
    }

    void transmit(int data, const char *name) {
      JsonDocument doc;

      doc["path"] = name;
      doc["data"] = data;

      serializeJson(doc, Serial);
      Serial.println();
    }

};

#endif

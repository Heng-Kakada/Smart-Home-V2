#include "Arduino.h"
#include "HardwareSerial.h"
#ifndef IRSENSOR_H
#define IRSENSOR_H

class IRSensor{
private:
    int pin;
    int sensorValue;
    bool detected = false;
public:
    IRSensor(int p) : pin(p), sensorValue(0) {}

    void begin() {
        pinMode(pin, INPUT);
    }
    void update(const char *name) {
      sensorValue = digitalRead(pin);

      if (sensorValue == LOW && !detected) {
        transmit(1, name);
        detected = true;
      } 
      else if (sensorValue == HIGH && detected) {
        transmit(0, name);
        detected = false;
      }

    }
    
    int getData() {
        return sensorValue;
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

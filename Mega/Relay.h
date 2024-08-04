#ifndef RELAY_H
#define RELAY_H

#include<Arduino.h>

class Relay {
  private:
    int pin;
    int state;

  public:

    Relay(int pin) {
      this->pin = pin;
    }

    void begin() {
      pinMode(pin, OUTPUT);
    }

    void activate() {
      state = true;
      digitalWrite(pin, HIGH);
    }

    void deactivate() {
      state = false;
      digitalWrite(pin, LOW);
    }

    void setState(int state) {
      digitalWrite(pin, state);
    }

    void toggle(){
      state = !state;
      digitalWrite(pin, state ? HIGH : LOW);
    }

};

#endif

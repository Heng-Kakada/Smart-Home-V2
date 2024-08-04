#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <Servo.h>

class ServoControl {
private:
    int pin;
    Servo servo;
    int open;
    int close;
public:
    ServoControl(int p,int open, int close){
      this->pin = p;
      this->open = open;
      this->close = close;
    }

    void begin() {
        servo.attach(pin);
        deactivate();
    }

    void activate() {
        servo.write(open);
    }
    void deactivate() {
        servo.write(close);
    }

    void setState(int state){
      if(state == 1){
        activate();
      }else{
        deactivate();
      }
    }

};

#endif

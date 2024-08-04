
#ifndef MOTOR_H
#define MOTOR_H

class Motor {
private:
    int enablePin;
    int inputPin1;
    int inputPin2;
public:
    int open_duration;
    int close_duration;

    Motor(int enPin, int inPin1, int inPin2, int od, int cd) {
        enablePin = enPin;
        inputPin1 = inPin1;
        inputPin2 = inPin2;
        open_duration = od;
        close_duration = cd;
    }

    void begin() {
        pinMode(enablePin, OUTPUT);
        pinMode(inputPin1, OUTPUT);
        pinMode(inputPin2, OUTPUT);
    }

    void forward(int speed) {
        analogWrite(enablePin, speed);
        digitalWrite(inputPin1, HIGH);
        digitalWrite(inputPin2, LOW);
    }

    void backward(int speed) {
        analogWrite(enablePin, speed);
        digitalWrite(inputPin1, LOW);
        digitalWrite(inputPin2, HIGH);
    }

    void stop() {
        digitalWrite(inputPin1, LOW);
        digitalWrite(inputPin2, LOW);
    }
};


#endif

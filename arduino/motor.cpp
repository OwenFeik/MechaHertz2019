#include "motor.h"
#include "Arduino.h"

Motor::Motor(int pin1, int pin2) {
    pin_1 = pin1;
    pin_2 = pin2;

    pinMode(pin_1, OUTPUT);
    pinMode(pin_2, OUTPUT);
}

void Motor::drive(int speed) {
    if (speed == 0) {
        stop();
    }
    else {
        bool direction;
        if (speed < 0) {
            direction = false; 
        }
        else {
            direction = true;
        }
        
        speed = map(abs(speed), 0, 100, 0, 255);

        if (direction) {
            digitalWrite(pin_1, LOW);
            analogWrite(pin_2, speed);
        }
        else {
            analogWrite(pin_1, speed);
            digitalWrite(pin_2, LOW);
        }
    }
}

void Motor::stop() {
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, LOW);
}

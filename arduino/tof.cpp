#include "tof.h"
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

Tof::Tof(int _shutdown_pins[4]) {
    sensors = new Adafruit_VL53L0X[4] {Adafruit_VL53L0X(), Adafruit_VL53L0X(), Adafruit_VL53L0X(), Adafruit_VL53L0X()};
    shutdown_pins = _shutdown_pins;
}

/*
    Configure pinouts and set I2C address for each sensor. 
*/
void Tof::init() {
    for (int i = 0; i < 4; i++) {
        pinMode(shutdown_pins[i], OUTPUT);
    }
    resetSensors();

    for (int i = 0; i < 4; i++) {
        digitalWrite(shutdown_pins[i], HIGH);
        delay(10);
        sensors[i].begin(0x30 + i); //Addresses 0x30 through 0x33
    }
}

/*
    Clear sensors and put all sensors except the first one into shutdown.
*/
void Tof::resetSensors() {
    for (int i = 0; i < 4; i++) {
       digitalWrite(shutdown_pins[i], LOW); 
    }

    delay(10);

    for (int i = 0; i < 4; i++) {
       digitalWrite(shutdown_pins[i], HIGH); 
    }

    delay(10);

    for (int i = 1; i < 4; i++) {
       digitalWrite(shutdown_pins[i], LOW);
    } //Leave 0 on so that it can be addressed
}

/*
    Update all range values.
    -1 denotes out of range.
*/
void Tof::update() {
    for (int i = 0; i < 4; i++) {
       sensors[i].rangingTest(&measures[i], false);
       
        if (measures[i].RangeStatus == 4 || measures[i].RangeMilliMeter == 8191) {
            readings[i] = -1;
        }
        else {
            readings[i] = measures[i].RangeMilliMeter;
        }
    }

    front = readings[0];
    left = readings[1];
    right = readings[2];
    back = readings[3];
}

void Tof::print_distances() {
    Serial.print("F: ");
    Serial.print(readings[0]);
    Serial.print(" L: ");
    Serial.print(readings[1]);
    Serial.print(" R: ");
    Serial.print(readings[2]);
    Serial.print(" B: ");
    Serial.print(readings[3]);
    Serial.println();
}

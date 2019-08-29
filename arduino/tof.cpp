#include <Arduino.h>
#include "tof.h"
#include "Adafruit_VL53L0X.h"

Tof::Tof(int _shutdown_pins[4]) {
    sensors = new Adafruit_VL53L0X[4] {Adafruit_VL53L0X(), Adafruit_VL53L0X(), Adafruit_VL53L0X(), Adafruit_VL53L0X()};
    shutdown_pins = _shutdown_pins;
}

/*
    Configure pinouts and set I2C address for each sensor. 
*/
void Tof::init(bool debug) {
    for (int i = 0; i < 4; i++) {
        // _front[i] = -1; _left[i] = -1; _right[i] = -1; _back[i] = -1;

        pinMode(shutdown_pins[i], OUTPUT);
    }
    _resetSensors();

    for (int i = 0; i < 4; i++) {
        digitalWrite(shutdown_pins[i], HIGH);
        delay(10);

        //Addresses 0x30 through 0x33
        while (!sensors[i].begin(0x30 + i)) {
            if (debug) {
                Serial.print("Tof ");
                Serial.print(i);
                Serial.println(" failed.");
            }
            delay(10);
        }
    }
}

/*
    Clear sensors and put all sensors except the first one into shutdown.
*/
void Tof::_resetSensors() {
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
    Always update oldest reading in history by rotating the index the 
    reading is saved to.
*/
void Tof::_rotateReadings() {
    if (_readingIndex == (_readingHistorySize - 1)) {
        _readingIndex = 0;
    }
    else {
        _readingIndex += 1;
    }
}

/*
    Bubble sorts readings to find the median value in order to reduce noise
    in readings.
*/
void Tof::_medianFilter() {
    for (int i = 0; i < 4; i++) {
        int values[_readingHistorySize] = {};
        memcpy(values, _readingHistory[i], _readingHistorySize); // Copy historical data into values

        bool swapped = true;
        while (swapped) {
            swapped = false;
            for (int i = 1; i < _readingHistorySize; i++) {
                if (values[i - 1] > values[i]) {
                    int val_i = values[i];
                    values[i] = values[i - 1];
                    values[i - 1] = val_i;
                    
                    swapped = true;
                }
            }
        }
        
        if (_readingHistorySize % 2 == 0) {
            _filteredReadings[i] = (values[_readingHistorySize / 2] + values[(_readingHistorySize / 2) - 1]) / 2;            
        }
        else {
            _filteredReadings[i] = values[(_readingHistorySize - 1) / 2];
        }
    }
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

        _readingHistory[i][_readingIndex] = readings[i];
    }

    _rotateReadings(); // Replace oldest readings each time
    _medianFilter(); // Take the middle value of the stored readings to reduce noise

    front = _filteredReadings[0];
    left = _filteredReadings[1];
    right = _filteredReadings[2];
    back = _filteredReadings[3];
}

void Tof::print_distances() {
    Serial.print("F: ");
    Serial.print(front);
    Serial.print(" L: ");
    Serial.print(left);
    Serial.print(" R: ");
    Serial.print(right);
    Serial.print(" B: ");
    Serial.print(back);
    Serial.println();
}

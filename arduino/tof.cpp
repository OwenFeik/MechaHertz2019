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
    Note: does not work for even _readingHistorySize because I was tired when I wrote
    this and couldn't be bothered to fix it. Shouldn't be too complex to do so. 

    Later note: works well for size 3, can't guarantee anything beyond that because I 
    really need to go to bed.

    Sorry future Owen - Owen.
*/
void Tof::_medianFilter() {

    for (int i = 0; i < 4; i++) {
        int max_value = 0;
        int min_value = 0;
        int mid_value = 0;

        for (int j = 0; j < _readingHistorySize; j++) {
            int val = _readingHistory[i][j];
            
            if (val >= max_value) {
                if (max_value > 0) {
                    if (mid_value > 0) {
                        if (mid_value < min_value || min_value == 0) {
                            min_value = mid_value;
                            mid_value = max_value;
                        }                        
                        else {
                            mid_value = (mid_value + max_value) / 2;
                        }                        
                    }
                    else {
                        mid_value = max_value;
                    }
                }

                max_value = val; 
            }
            else if (val <= min_value || min_value == 0) {
                if (min_value > 0) {
                    if (mid_value > 0) {
                        mid_value = (mid_value + min_value) / 2;
                    }
                    else {
                        mid_value = min_value;
                    }
                }
                
                min_value = val;
            }
            else {
                if (mid_value > 0) {
                    mid_value = (mid_value + val) / 2;
                }
                else {
                    mid_value = val;
                }
            }
        }

        _filteredReadings[i] = mid_value;
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

    _rotateReadings();

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

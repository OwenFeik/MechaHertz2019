#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include "gyro.h"

#define to_degrees 57.295779513082 // This should be defined rather than a class property.

Gyro::Gyro() {
    gyro = new Adafruit_L3GD20_Unified();
}

void Gyro::init(bool debug) {
    gyro->enableAutoRange(true);
    while (!gyro->begin()) {
        if (debug) {
            Serial.println("Gyro failed.");
        }
        delay(10);
    }

    time = millis();

    calibrate();
}

void Gyro::calibrate(int readings) {
    float _x_sum = 0, _y_sum = 0, _z_sum = 0;

    for (int i = 0; i < readings; i++) {
        read();

        _x_sum += x;
        _y_sum += y;
        _z_sum += z;

        delay(10);
    }
    
    x_offset = _x_sum / readings;
    y_offset = _y_sum / readings;
    z_offset = _z_sum / readings;

    use_offset = true;
    time = millis();
}

void Gyro::read() {
    sensors_event_t event;
    gyro->getEvent(&event);

    if (use_offset) {
        x = event.gyro.x - x_offset;
        y = event.gyro.y - y_offset;
        z = event.gyro.z - z_offset;
    }
    else {
        x = event.gyro.x;
        y = event.gyro.y;
        z = event.gyro.z;
    }
}

void Gyro::update() {
    read();

    unsigned long _time = millis();
    float t_delta = (_time - time) / 1000;

    if (t_delta > calculation_interval) {

        float h_delta = (t_delta) * (z_sum / z_readings) * to_degrees;

        // != checks for NaN
        if (!(h_delta != h_delta)) {
            heading += h_delta;
            if (heading > 360) {
                heading -= 360;
            }
            else if (heading < 0) {
                heading += 360;
            }
        }

        time = _time;

        z_sum = 0;
        z_readings = 0;
    }
    else {
        z_sum += z;
        z_readings += 1;
        
    }
}

/*
    Return true if the current bearing is within tolerance degrees
    of initial, false if not.
*/
bool Gyro::facingForward(int tolerance) {
    if (heading > 360 - tolerance || heading < 0 + tolerance) {
        return true;
    }
    else {
        return false;
    }
}

/*

// This code only updates the heading very rarely. Too infrequently to be useful.
// Even when I drop the "sample_interval" to 10 its quite slow. I think the more
// real-time readings from the above implementation compensate for the small amount
// of drift.

void Gyro::update(){
    read();
    // Drift correction since z_offest may change throughout this prevents the heading
    //   from drifting when the robot is still. However, the robot may actually be moving
    //   very slowly, so calibrate range to one that is most suitable
    if (z < -error_range && z > error_range) { z_sum += 0; } //if within error range, round to 0
    else { z_sum += z; }
    
    //Get total dist. travelled from average velocity
    if (z_readings == sample_interval - 1){ //because readings start at 0, loop 0 - 99 instead of 1 - 100
        // best if time interval variables are the closest
        unsigned long _time = millis();
        float t_delta = (_time - time) / 1000;

        heading += (t_delta) * (z_sum / sample_interval) * (to_degrees); 

        time = _time;

        //Adjustment to 360 deg. range
        if (heading > 360) { heading -= 360; }
        else if (heading < 0) { heading += 360; }

        z_sum = 0;
        z_readings = 0;
    } 
    else { z_readings += 1; }
}
*/

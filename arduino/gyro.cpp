#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include "gyro.h"

Gyro::Gyro() {
    gyro = new Adafruit_L3GD20_Unified();
}

void Gyro::init() {
    gyro->enableAutoRange(true);
    gyro->begin();

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
    float delta_t = (_time - time) / 1000;

    if (delta_t > 0.1) {

        // Serial.print("delta_t/1000: "); Serial.print(delta_t);
        // Serial.print(" z_sum/z_readings: "); Serial.println(z_sum / z_readings);

        heading += (delta_t) * (z_sum / z_readings) * (180 / 3.1415926535); 
        if (heading > 360) {
            heading -= 360;
        }
        else if (heading < 0) {
            heading += 360;
        }
        time = _time;

        z_sum = 0;
        z_readings = 0;
    }
    else {
        z_sum += z;
        z_readings += 1;
        
        // Serial.print("z_sum: "); Serial.print(z_sum);
        // Serial.print(" z_readings: "); Serial.println(z_readings);
    }

}

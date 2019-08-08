#include "colour.h"
#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "tof.h"
#include "gyro.h"
// #include "compass.h"

int tof_shutdown_pins[] = {30, 31, 32, 33};

Colour colour = Colour();
Pixy pixy = Pixy();
Drive drive = Drive(2, 3, 4, 5, 6, 7);
Toggle toggle = Toggle(24, 25);
Tof tof = Tof(tof_shutdown_pins);
Gyro gyro = Gyro();
// Compass compass = Compass();

int state = 0; // Toggle switch state

void update_all() {
    colour.update();
    pixy.update();
    state = toggle.getState();
    tof.update();
    gyro.update();
    // compass.update();
}

void setup() {
    Serial.begin(115200);
    tof.init();
}

void loop() {
    update_all();

    if (state == 1) {
        if (pixy.visible) {
            if (pixy.y < 20) {
                drive.stop();
                // drive.orbit(100, pixy.x, gyro.heading);
            }
            else {
                drive.chase(100, pixy.x);
            }
        }
        else if (pixy.last_seen < 60) {
            if (pixy.x > 80) {
                drive.turn(50);
            }
            else if (pixy.x < 20) {
                drive.turn(-50);
            }
        }
        else {
            drive.stop();
        }
    }
    else if (state == 2) {
        drive.stop();

        for (int i = 0; i < 4; i++) {
            Serial.print(tof.readings[i]); Serial.print(' ');
        }
        Serial.println();

    }
    else {
        drive.stop();
    }
}

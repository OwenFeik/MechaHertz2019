// #include "colour.h"
// #include "compass.h"
// #include "tof.h"
#include "drive.h"
#include "pixy.h"
#include "toggle.h"
#include "gyro.h"

// int tof_shutdown_pins[] = {30, 31, 32, 33};

// Colour colour = Colour();
Pixy pixy = Pixy();
// Compass compass = Compass();
Drive drive = Drive(2, 3, 4, 5, 6, 7);
Toggle toggle = Toggle(30, 31);
// Tof tof = Tof(tof_shutdown_pins);
Gyro gyro = Gyro();

void setup() {
    Serial.begin(115200);

    gyro.init();
}

void loop() {
    int state = toggle.getState();
    pixy.update();
    gyro.update();

    if (state == 1) {
        if (pixy.visible) {
            if (pixy.y < 20) {
                drive.stop();
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

        
    }
    else {
        drive.stop();
    }
}

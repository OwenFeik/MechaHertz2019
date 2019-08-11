#include "colour.h"
#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "tof.h"
// #include "gyro.h"
// #include "compass.h"

int tof_shutdown_pins[] = {30, 31, 32, 33}; // Front, Left, Right, Back

Colour colour = Colour(); // 0x29
Pixy pixy = Pixy();
Drive drive = Drive(2, 3, 4, 5, 6, 7);
Toggle toggle = Toggle(24, 25);
Tof tof = Tof(tof_shutdown_pins); // 0x29, switched over to 0x30 through 0x33 when tof.init() is called.
// Gyro gyro = Gyro(); // 0x6B
// Compass compass = Compass();

int state = 0; // Toggle switch state

void update_all() {
    colour.update();
    pixy.update();
    state = toggle.getState();
    tof.update();
    // gyro.update();
    // compass.update();
}

void setup() {
    Serial.begin(115200);

    pinMode(40, OUTPUT); // Colour sensor pin
    digitalWrite(40, LOW); // Colour sensor off
    delay(10);

    tof.init(); // Set new tof addresses

    digitalWrite(40, HIGH); // Colour sensor on
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
        if ((abs(tof.left - tof.right) / ((tof.left + tof.right) / 2)) > 0.1) {
            if (tof.left > tof.right) {
                drive.strafe(-100);
            }
            else {
                drive.strafe(100);
            }
        }
    }
    else {
        drive.stop();
    }
}

#include "colour.h"
#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "tof.h"
#include "gyro.h"
// #include "compass.h"

#define BOT 1
#define DEBUG true

#if BOT == 1 // Original bot
    int tof_shutdown_pins[] = {30, 31, 32, 33}; // Front, Left, Right, Back
    int colour_power_pin = 40;
    Toggle toggle = Toggle(24, 25);
#elif BOT == 2 // Second bot
    int tof_shutdown_pins[] = {14, 15, 16, 17}; // Front, Left, Right, Back
    int colour_power_pin = 8;
    Toggle toggle = Toggle(12, 13);
#endif

Colour colour = Colour(); // 0x29
Pixy pixy = Pixy();
Drive drive = Drive(2, 3, 4, 5, 6, 7);
Tof tof = Tof(tof_shutdown_pins); // 0x29, switched over to 0x30 through 0x33 when tof.init() is called.
Gyro gyro = Gyro(); // 0x6B
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
    if (DEBUG) {
        Serial.begin(115200);
        Serial.println("Begin serial: ");
    }
    
    gyro.init();

    if (DEBUG) {
        Serial.println("Gyro successfully booted.");
    }

    pinMode(colour_power_pin, OUTPUT); // Colour sensor pin
    digitalWrite(colour_power_pin, LOW); // Colour sensor off
    delay(10);

    tof.init(true); // Set new tof addresses

    if (DEBUG) {
        Serial.println("TOF successfully booted.");
    }

    digitalWrite(colour_power_pin, HIGH); // Colour sensor on
}

void loop() {
    update_all();

    // Try to chase ball
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
    } // Try to centre self.
    else if (state == 2) {
        if (tof.left == -1 || tof.right == -1) {
            drive.stop();
        }
        else if (tof.left / tof.right > 1.05) {
            drive.strafe(-100, gyro.heading);
        }
        else if (tof.right / tof.left > 1.05) {
            drive.strafe(100, gyro.heading);
        }
        else {
            drive.stop();
        }
    }
    else {
        drive.stop();
    }
}

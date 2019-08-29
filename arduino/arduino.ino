#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "tof.h"
#include "gyro.h"
// #include "colour.h"
// #include "compass.h"

#define BOT 1
#define DEBUG false

#if BOT == 1 // Original bot
    int tof_shutdown_pins[] = {30, 31, 32, 33}; // Front, Left, Right, Back
    // int colour_power_pin = 40; // Colour sensor not in use
    Toggle toggle = Toggle(24, 25);
#elif BOT == 2 // Second bot
    int tof_shutdown_pins[] = {14, 15, 16, 17}; // Front, Left, Right, Back
    // int colour_power_pin = 8; // Colour sensor not in use
    Toggle toggle = Toggle(12, 13);
#endif

Pixy pixy = Pixy();
Drive drive = Drive(2, 3, 4, 5, 6, 7);
Tof tof = Tof(tof_shutdown_pins); // 0x29, switched over to 0x30 through 0x33 when tof.init() is called.
Gyro gyro = Gyro(); // 0x6B
// Colour colour = Colour(); // 0x29 // Not in use
// Compass compass = Compass(); // Not in use

bool panning = false;
bool panning_dir = false;
int state = 0; // Toggle switch state

void update_all() {
    pixy.update();
    state = toggle.getState();
    tof.update();
    gyro.update();
    // colour.update();
    // compass.update();

    #if DEBUG
        tof.printDistances();
        Serial.print("Pixy: "); Serial.print(pixy.x);
        Serial.print(" Gyro: "); Serial.println(gyro.heading);
    #endif
}

void setup() {

    #if DEBUG
        Serial.begin(115200);
        Serial.println("Begin serial: ");
    #endif
    
    gyro.init();

    #if DEBUG
        Serial.println("Gyro successfully booted.");
    #endif

    // pinMode(colour_power_pin, OUTPUT); // Colour sensor pin
    // digitalWrite(colour_power_pin, LOW); // Colour sensor off
    // delay(10);

    #if DEBUG
        tof.init(true); // Set new tof addresses
        Serial.println("TOF successfully booted.");
    #else
        tof.init(false);
    #endif

    // digitalWrite(colour_power_pin, HIGH); // Colour sensor on
}

void loop() {
    update_all();

    // Switch forward -> attacker
    if (state == 1) {
        if (pixy.visible && gyro.facingForward(45)) {
            panning = false;

            if (pixy.y < 50 || (pixy.x > 40 && pixy.x < 60)) {
                drive.chase(100, pixy.x);
            }
            else {
                if (pixy.x > 50) {
                    drive.strafe(100, gyro.heading);
                }
                else {
                    drive.strafe(-100, gyro.heading);
                }
            }
        }
        else if (pixy.last_seen < 50 && gyro.facingForward(45)) {
            panning = false;

            if (pixy.x < 50) {
                drive.turn(-80);
            }
            else if (pixy.x > 50) {
                drive.turn(80);
            }
        }
        else if (gyro.facingForward(20)) {
            if (tof.left > 400) {
                drive.strafe(-100, gyro.heading);
            }
            else if (tof.back > 500) {
                drive.go(-100);
            }
            else {
                panning = true;
            }
        }
        else if (panning) {
            if (!(gyro.heading < 335 && gyro.heading > 295)) {
                if (gyro.heading < 295 && gyro.heading > 135) {
                    drive.turn(-80);
                }
                else {
                    drive.turn(80);
                }
            }
        }
        else {
            drive.forward(100, gyro.heading);
        }
        
        // Old (working) chase implementation
        // if (pixy.visible) {
        //     if (pixy.y < 20) {
        //         drive.stop();
        //     }
        //     else {
        //         drive.chase(100, pixy.x);
        //     }
        // }
        // else if (pixy.last_seen < 60) {
        //     if (pixy.x > 80) {
        //         drive.turn(50);
        //     }
        //     else if (pixy.x < 20) {
        //         drive.turn(-50);
        //     }
        // }
        // else {
        //     drive.stop();
        // }
    }
    // Switch back -> goalie
    else if (state == 2) {
        if (tof.back > 800) {
            if (gyro.facingForward(30)) {
                drive.go(-100);                    
            }
            else {
                drive.forward(100, gyro.heading);
            }

            panning = false;
        }
        else if (pixy.visible) {
            if (pixy.y <= 30 && pixy.x > 30 && pixy.x < 70) {
                drive.chase(100, pixy.x);
            }
            else {
                drive.face(100, pixy.x);
            }

            panning = false;
        }
        else if (tof.back > 300) {
            if (gyro.facingForward(30)) {
                drive.go(-80);
            }
            else {
                drive.forward(100, gyro.heading);
            }

            panning = false;
        }
        else if (pixy.last_seen < 50) {
            if (pixy.x < 50) {
                drive.turn(-80);
            }
            else if (pixy.x > 50) {
                drive.turn(80);
            }

            panning = false;
        }
        else if (!panning && !gyro.facingForward(30)) {
            drive.forward(100, gyro.heading);
        }
        else if (!panning && tof.off_center) {
            if (tof.left > tof.right) {
                drive.strafe(80, gyro.heading);
            }
            else if (tof.right > tof.left) {
                drive.strafe(-80, gyro.heading);
            }
        }
        else if (panning) {
            if (gyro.heading > 180 && gyro.heading < 300) {
                panning_dir = true;
            }
            else if (gyro.heading < 180 && gyro.heading > 60) {
                panning_dir = false;
            }

            if (panning_dir) {
                drive.turn(-50);
            }
            else {
                drive.turn(50);
            }
        }
        else {
            panning = true;
        }
    }
    else {
        drive.stop();
    }
}

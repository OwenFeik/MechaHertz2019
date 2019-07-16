#include "drive.h"
#include <Arduino.h> // 'map' function

/*
Constructor
Accepts 3 pins, which should be attached to the ESCs
*/
Drive::Drive(int mot_1_pin_1, int mot_1_pin_2, int mot_2_pin_1, int mot_2_pin_2, int mot_3_pin_1, int mot_3_pin_2) {    
    mot_left = new Motor(mot_1_pin_1, mot_1_pin_2);
    mot_right = new Motor(mot_2_pin_1, mot_2_pin_2);
    mot_rear = new Motor(mot_3_pin_1, mot_3_pin_2);
}

/*  
Turn the robot left or right.
speed: -100-100
*/  
void Drive::turn(int speed) {
    mot_left->drive(speed);
    mot_right->drive(speed);
    mot_rear->drive(speed);
}

/*
Drive the robot forward or backward.
speed: 0-100
*/
void Drive::go(int speed) {
    mot_left->drive(speed);
    mot_right->drive(-1 * speed);
    mot_rear->drive(0);
}

/*
Strafe the bot to the left or right.
speed: 0-100
*/
void Drive::strafe(int speed) {
    mot_left->drive(speed);
    mot_right->drive(speed);
    mot_rear->drive(-1 * speed);
}

void Drive::stop() {
    mot_left->stop();
    mot_right->stop();
    mot_rear->stop();
}

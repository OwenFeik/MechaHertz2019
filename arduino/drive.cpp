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

void Drive::drive(int speed_left, int speed_right, int speed_rear) {
    mot_left->drive(speed_left);
    mot_right->drive(speed_right);
    mot_rear->drive(speed_rear);
}

/*  
Turn the robot left or right.
speed: -100 - 100
*/  
void Drive::turn(int speed) {
    drive(speed, speed, speed);
}

/*
Drive the robot forward or backward.
speed: -100 - 100
*/
void Drive::go(int speed) {
    drive(speed, -1 * speed, 0);
}

/*
Strafe the bot to the left or right.
speed: -100 - 100
*/
void Drive::strafe(int speed, float heading) {
    float left_multiplier = 0.5;
    float right_multiplier = 0.5;

    if (heading > 180 && heading < 350) {
        if (heading > 340) {
            left_multiplier = 0.45;
            right_multiplier = 0.55;
        }
        else if (heading > 330) {
            left_multiplier = 0.4;
            right_multiplier = 0.6;
        }
        else {
            left_multiplier = 0.3;
            right_multiplier = 0.7;
        }
    }
    else if (heading < 180 && heading > 10) {
        if (heading < 20) {
            right_multiplier = 0.45;
            left_multiplier = 0.55;
        }
        else if (heading < 30) {
            right_multiplier = 0.4;
            left_multiplier = 0.6;
        }
        else {
            right_multiplier = 0.3;
            left_multiplier = 0.7;
        }
    }

    drive(left_multiplier * speed, right_multiplier * speed, -1 * speed);
}

void Drive::stop() {
    drive(0, 0, 0);
}

/* 
Chase the ball; move forward while turning to face it head on.
speed: 0-100
x: proportional x position of the ball (output from Pixy object)
*/
void Drive::chase(int speed, float x) {
    if (x > 30 && x < 70) {
        go(speed);
    }
    else {
        float multiplier = 0.7;
        if (x > 20 && x < 80) {
            multiplier = 0.55;
        }
        else if (x > 10 && x < 90) {
            multiplier = 0.4;
        }
        else {
            multiplier = 0.25;
        }

        if (x < 50) {
            drive(int(speed * multiplier), -speed, 0);
        }
        else {
            drive(speed, -int(speed * multiplier), 0);
        }
    }
}

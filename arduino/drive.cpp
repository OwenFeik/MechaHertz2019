#include "drive.h"
#include <Arduino.h> // 'map' function

/*
Constructor
Accepts 3 pins, which should be attached to the ESCs
*/
Drive::Drive(int pin_left, int pin_right, int pin_rear) {    
    mot_left.attach(pin_left, 1000, 2000); // Pulse time 1-2ms
    mot_right.attach(pin_right, 1000, 2000);
    mot_rear.attach(pin_rear, 1000, 2000);
}

/*
Servo.write() accepts 0-180, so
Remap speed from directed 0-100 to 0-180
0-90 is one direction, 90+ is the other
*/
int Drive::remap_speed(bool dir, int speed) {
    speed = map(speed, 0, 100, 0, 90);
    if (!dir) {
        speed *= -1;
    }
    speed += 90;

    return speed;
}

/*  
Turn the robot left or right.
dir: true for clockwise (right), false for anticlockwise (left)
speed: 0-100
*/  
void Drive::turn(bool dir, int speed) {
    speed = remap_speed(dir, speed);

    mot_left.write(speed);
    mot_right.write(speed);
    mot_rear.write(speed);
}

/*
Drive the robot forward or backward.
dir: true for forward, false for backward
speed: 0-100
*/
void Drive::go(bool dir, int speed) {
    speed = remap_speed(dir, speed);
    
    mot_left.write(speed);
    mot_right.write(180 - speed);
}

/*
Strafe the bot to the left or right.
dir: true for right, false for left
speed: 0-100
*/
void Drive::strafe(bool dir, int speed) {
    speed = remap_speed(dir, speed);

    mot_left.write(speed);
    mot_right.write(speed);
    mot_rear.write(180 - speed);
}

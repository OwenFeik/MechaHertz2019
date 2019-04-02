#include "drive.h"
#include <Arduino.h> // 'map' function

/*
Constructor
Accepts 3 pins, which should be attached to the ESCs
*/
Drive::Drive(int pin_left,int pin_right,int pin_rear){    
    this->mot_left.attach(pin_left,1000,2000);
    this->mot_right.attach(pin_right,1000,2000);
    this->mot_rear.attach(pin_rear,1000,2000);
}

/*  
Turn the robot left or right.
dir: true for clockwise, false for anticlockwise
speed: 0-100
*/  
void Drive::turn(bool dir, int speed){
    speed=map(speed,0,100,0,90); // Servo.write() accepts 0-180
    if (!dir){
        speed*=-1;
    }
    speed+=90; // 0-90 is one direction, 90+ is the other

    this->mot_left.write(speed);
    this->mot_right.write(speed);
    this->mot_rear.write(speed);
}

/*
Drive the robot forward or backward.
dir: true for forward, false for backward
speed: 0-100
*/
void Drive::go(bool dir, int speed){
    speed=map(speed,0,100,0,90); // Servo.write() accepts 0-180
    if (!dir){
        speed*=-1;
    }
    speed+=90; // 0-90 is one direction, 90+ is the other
    
    this->mot_left.write(speed);
    this->mot_right.write(180-speed);
}

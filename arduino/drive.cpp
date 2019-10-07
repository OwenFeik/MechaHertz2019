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
    if (x > 35 && x < 65) {
        go(speed);
    }
    else {
        float multiplier = 0.7;
        if (x > 25 && x < 75) {
            multiplier = 0.55;
        }
        else if (x > 15 && x < 85) {
            multiplier = 0.4;
        }
        else {
            face(speed, x);
        }

        if (x < 50) {
            drive(int(speed * multiplier), -speed, 0);
        }
        else {
            drive(speed, -int(speed * multiplier), 0);
        }
    }
}

void Drive::face(int speed, float x) {
    int _speed = abs(speed);

    if (x > 45 && x < 55) {
        stop();
    }
    else if (x < 50) {
        float multiplier = 1;
        if (x > 40) {
            multiplier = 0.25;
        }
        else if (x > 30) {
            multiplier = 0.5;
        }
        else if (x > 20) {
            multiplier = 0.7;
        }

        turn(int(multiplier * _speed * -1));
    }
    else {
        float multiplier = 1;
        if (x < 60) {
            multiplier = 0.25;
        }
        else if (x < 70) {
            multiplier = 0.5;
        }
        else if (x < 80) {
            multiplier = 0.7;
        }

        turn(int(multiplier * _speed));
    }
}

void Drive::forward(int speed, float heading) {
    if (heading > 340 || heading < 20) {
        stop();
    }
    else if (heading < 180) {
        float multiplier = 1;
        if (heading < 40) {
            multiplier = 0.25;
        }
        else if (heading < 60) {
            multiplier = 0.5;
        }
        else if (heading < 80) {
            multiplier = 0.7;
        }

        turn(speed * multiplier);
    }
    else {
        float multiplier = 1;
        if (heading > 320) {
            multiplier = 0.25;
        }
        else if (heading < 300) {
            multiplier = 0.5;
        }
        else if (heading < 280) {
            multiplier = 0.7;
        }

        turn(-1 * speed * multiplier);
    }
}

#define PI 3.14159

void Drive::euclid(double bearing, int speed)
{    
    double xvalue = cos(bearing / 180 * PI);
    double yvalue = sin(bearing / 180 * PI);
    
    double motor1 = yvalue * cos(35  * PI / 180) - xvalue * sin(35  * PI / 180);
    double motor2 = yvalue * cos(145 * PI / 180) - xvalue * sin(145 * PI / 180);
    double motor3 = yvalue * cos(215 * PI / 180) - xvalue * sin(215 * PI / 180);
    double motor4 = yvalue * cos(325 * PI / 180) - xvalue * sin(325 * PI / 180);
    
    double balance = 1/fmax(fmax((motor1), (motor2)), fmax((motor3), (motor4)));
    
    motor1 *= balance;
    motor2 *= balance;
    motor3 *= balance;
    motor4 *= balance;
    
    std::cout << "Front R: ";
    std::cout << speed * motor4;
    std::cout << " Front L: ";
    std::cout << speed * motor1;
    std::cout << " Back R: ";
    std::cout << speed * motor3;
    std::cout << " Back L: ";
    std::cout << speed * motor2;
}
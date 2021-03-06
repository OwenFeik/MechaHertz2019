#include "drive.h"
#include <Arduino.h> // 'map' function

/*
Constructor
Accepts the two pins of each of the four motors
*/
Drive::Drive(int mot_FR_pin_1, int mot_FR_pin_2, int mot_FL_pin_1, int mot_FL_pin_2, int mot_BR_pin_1, int mot_BR_pin_2, int mot_BL_pin_1, int mot_BL_pin_2) {    
    mot_FR = new Motor(mot_FR_pin_1, mot_FR_pin_2);
    mot_FL = new Motor(mot_FL_pin_1, mot_FL_pin_2);
    mot_BR = new Motor(mot_BR_pin_1, mot_BR_pin_2);
    mot_BL = new Motor(mot_BL_pin_1, mot_BL_pin_2);
}

void Drive::drive(int speed_FR, int speed_FL, int speed_BR, int speed_BL) {
    mot_FR->drive(speed_FR);
    mot_FL->drive(speed_FL);
    mot_BR->drive(speed_BR);
    mot_BL->drive(speed_BL);
}

void Drive::euclid(float bearing, int speed, float rotation)
{    
    float xvalue = cos(bearing * DEG_TO_RAD);
    float yvalue = sin(bearing * DEG_TO_RAD);
    
    float mot_FL = yvalue * 0.819 - xvalue * 0.574; // mot_FL -> 35*
    float mot_BL = yvalue * -1 * 0.819 - xvalue * 0.574; // mot_BL -> 145*
    float mot_BR = yvalue * -1 * 0.819 - xvalue * -1 * 0.574; // mot_BR -> 215*
    float mot_FR = yvalue * 0.819 - xvalue * -1 * 0.574; // mot_BR -> 325*
    
    float balance = speed/max(max((mot_FL), (mot_BL)), max((mot_BR), (mot_FR)));
    
    mot_FL = (balance * mot_FL) + rotation;
    mot_BL = (balance * mot_BL) + rotation;
    mot_BR = (balance * mot_BR) + rotation;
    mot_FR = (balance * mot_FR) + rotation;
    
    drive(mot_FR, mot_FL, mot_BR, mot_BL);
}

void Drive::stop() {
    drive(0, 0, 0, 0);
}

void Drive::go(int speed) {
    euclid(0, speed);
}

/*  
Turn the robot left or right.
speed: -100 - 100
*/
/* 
void Drive::turn(int speed) {
    drive(speed, speed, speed);
}
*/

/*
Drive the robot forward or backward.
speed: -100 - 100
*/

/*
Strafe the bot to the left or right.
speed: -100 - 100
*/
/*
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
*/

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
        int offset = 0;
        if (x > 25 && x < 75) {
            offset = 20;
        }
        else if (x > 15 && x < 85) {
            offset = 10;
        }

        if (x < 50) {
            euclid(50 - offset, speed);
        }
        else {
            euclid(-50 + offset, speed);
        }
    }
}



void Drive::face(int speed, float x) {
    int _speed = abs(speed);

    if (x > 45 && x < 55) {
        stop();
    }
    else if (x < 50) {
        int offset = 0;
        if (x > 40) {
            offset = 50;
        }
        else if (x > 30) {
            offset = 25;
        }
        else if (x > 20) {
            offset = 10;
        }

        euclid(0, 0, 100 - offset);
    }
    else {
        int offset = 0;
        if (x > 60) {
            offset = 50;
        }
        else if (x > 70) {
            offset = 25;
        }
        else if (x > 80) {
            offset = 10;
        }

        euclid(0, 0, -100 + offset);
    }
}


/*
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
*/

#include "motor.h"

class Drive {
    public:
        Motor *mot_FR, *mot_FL, *mot_BR, *mot_BL;
    
    Drive(int mot_FR_pin_1, int mot_FR_pin_2, int mot_FL_pin_1, int mot_FL_pin_2, int mot_BR_pin_1, int mot_BR_pin_2, int mot_BL_pin_1, int mot_BL_pin_2);
    void drive(int speed_FR, int speed_FL, int speed_BR, int speed_BL);
    void euclid(float bearing, int speed, float rotation = 0);
    void go(int speed);
    void stop();
    
    //void turn(int speed);
    // void strafe(int speed, float heading = 0);
    void chase(int speed, float x);
    void face(int speed, float x);
    // void forward(int speed, float heading);
};

#include "motor.h"

class Drive {
    public:
        Motor mot_left, mot_right, mot_rear;
    
    Drive(int mot_1_pin_1, int mot_1_pin_2, int mot_2_pin_1, int mot_2_pin_2, int mot_3_pin_1, int mot_3_pin_2);
    int remap_speed(bool dir, int speed);
    void turn(int speed);
    void go(int speed);
    void strafe(int speed);
    void stop();
};

#include "motor.h"

class Drive {
    public:
        Motor *mot_left, *mot_right, *mot_rear;
    
    Drive(int mot_1_pin_1, int mot_1_pin_2, int mot_2_pin_1, int mot_2_pin_2, int mot_3_pin_1, int mot_3_pin_2);
    void drive(int speed_left, int speed_right, int speed_rear);
    void turn(int speed);
    void go(int speed);
    void strafe(int speed, float heading = 0);
    void stop();
    void chase(int speed, float x);
    void face(int speed, float x);
    void forward(int speed, float heading);
};

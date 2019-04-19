#include <Servo.h>

class Drive {
    public:
        Servo mot_left, mot_right, mot_rear;
    
    Drive(int pin_left, int pin_right, int pin_rear);
    int remap_speed(bool dir, int speed);
    void turn(bool dir, int speed);
    void go(bool dir, int speed);
    void strafe(bool dir, int speed);
};

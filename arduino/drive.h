#include <Servo.h>

class Drive{
    public:
        Servo mot_left,mot_right,mot_rear;
    
    Drive(int pin_left,int pin_right,int pin_rear);
    void turn(bool dir, int speed);
    void go(bool dir, int speed);
};

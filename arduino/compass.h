#include <Wire.h> // Library to interface with the magnetometer

class Compass{
    public:
        float x_offset, y_offset;
        int x, y, z;
        float heading;
        float initial_heading;

    Compass(); // Constructor
    void calibrateSelf(); // Calibrate the compass
    void updateXYZ(); // Update the public x,y,z ints
    float getHeading(); // Get the most up to date heading
    float getAccurateHeading(); // Average of 10 heading readings
};

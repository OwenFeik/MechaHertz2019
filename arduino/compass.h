#include <Wire.h> // Library to interface with the magnetometer

class Compass{
    public:
        float x_offset,y_offset;
        int x,y,z;
        float heading;
        float initial_heading;

    Compass();
    void calibrateSelf();
    void updateXYZ();
    float getHeading();
    float getAccurateHeading();
};

#include <Wire.h> // Library to interface with the magnetometer

class Compass {
    public:
        float x_bias, y_bias, z_bias;
        int x, y, z;
        float heading;
        float initial_heading; // The direction the bot initially faced, degrees true
        bool facing_goal; // Is the bot facing the starting direction?

    Compass(); // Constructor
    void calibrateSelf(); // Calibrate the compass
    void updateXYZ(); // Update the public x,y,z ints
    void update(); // Get the most up to date heading
    float getAccurateHeading(); // Average of 10 heading readings
};

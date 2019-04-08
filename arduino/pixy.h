#include <Pixy2.h>

class Pixy{
    public:
        Pixy2 pixy; // Pixy driver
        int x, y, width, height, age; // Percentage location of ball, width height of ball, number of frames on screen
        float heading, dist; // Heading of the ball from the robot in degrees, distance in pixels
        bool visible; // Was ball visible on last reading

    Pixy(); // Constructur
    void update(); // Update public fields with camera data
};

#include <Adafruit_L3GD20_U.h>

class Gyro {
    public:
        Adafruit_L3GD20_Unified *gyro;
        unsigned long time = 0; // Time since heading was last updated
        float x, y, z; // Readings from the sensor
        float z_sum = 0; // Total rotational distance since last calculation
        int z_readings = 0; // Number of rotational readings since last calculation
        bool use_offset = false; // If calibrated, deduct following offsets from readings
        float x_offset = 0, y_offset = 0, z_offset = 0;
        float heading = 0; // Calculated bearing of the robot, initially 0
        float calculation_interval = 0.1; // Update bearing every time this many seconds pass

    Gyro();
    void init(bool debug = false);
    void calibrate(int readings = 100);
    void read();
    void update();
    bool facingForward(int tolerance = 20);
};

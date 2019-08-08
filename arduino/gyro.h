#include <Adafruit_L3GD20_U.h>

class Gyro {
    public:
        Adafruit_L3GD20_Unified *gyro;
        unsigned long time = 0;
        float x, y, z;
        float z_sum = 0;
        int z_readings = 0;
        bool use_offset = false;
        float x_offset = 0, y_offset = 0, z_offset = 0;
        float heading = 0;
        float time_interval = 0.01;
        int sample_interval = 100;
        float error_range = 0.15;
        float to_degrees = 57.295779513082;

    Gyro();
    void init();
    void calibrate(int readings = 100);
    void read();
    void update();
};

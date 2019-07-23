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

    Gyro();
    void init();
    void calibrate(int readings = 100);
    void read();
    void update();
};

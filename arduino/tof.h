#include "Adafruit_VL53L0X.h"

class Tof {
    public:
        int* shutdown_pins;
        Adafruit_VL53L0X* sensors;
        VL53L0X_RangingMeasurementData_t measures[4];
        int readings[4];
        int front, left, right, back;

    Tof(int _shutdown_pins[4]);
    void init(bool debug = false);
    void resetSensors();
    void update();
    void print_distances();
};

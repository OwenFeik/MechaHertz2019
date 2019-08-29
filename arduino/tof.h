#include "Adafruit_VL53L0X.h"

#define _readingHistorySize 3

class Tof {
    public:
        int* shutdown_pins;
        Adafruit_VL53L0X* sensors;
        VL53L0X_RangingMeasurementData_t measures[4];
        int readings[4];
        int front, left, right, back;
        bool off_center = false;

    Tof(int _shutdown_pins[4]);
    void init(bool debug = false);
    void update();
    void printDistances();

    private:
        int _readingHistory[4][_readingHistorySize] = {}; // Keep reading history to apply median filter
        int _filteredReadings[4];
        int _readingIndex = 0; // Overwrite oldest value

    void _resetSensors();
    void _rotateReadings();
    void _medianFilter();
};

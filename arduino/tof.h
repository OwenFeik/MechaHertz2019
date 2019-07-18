#include "Adafruit_VL53L0X.h"

class Tof {
   public:
       Adafruit_VL53L0X sensors[4];
       VL53L0X_RangingMeasurementData_t measures[4];
       int shutdown_pins[4];

   Tof(int _shutdown_pins[4]);
   void resetSensors();
};

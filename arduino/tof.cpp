#include "tof.h"
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

Tof::Tof(int _shutdown_pins[4]) {
   shutdown_pins = _shutdown_pins;
   resetSensors();

   for (int i = 0, i < 4, i++) {
       digitalWrite(shutdown_pins[i], HIGH);
       
       int address = 0x30 + i; //Sensors get addresses 0x30 through 0x33

       sensors[i] = Adafruit_VL53L0X(address);
       sensors[i].begin(address);
   }
}

void Tof::resetSensors() {
   for (int i = 0, i < 4, i++) {
       pinMode(shutdown_pins[i], OUTPUT);
       digitalWrite(shutdown_pins[i], LOW); 
   }

   delay(10);

   for (int i = 0, i < 4, i++) {
       digitalWrite(shutdown_pins[i], HIGH); 
   }

   delay(10);

   for (int i = 1, i < 4, i++) {
       digitalWrite(shutdown_pins[i], LOW);
   } //Leave 0 on so that it can be addressed
}

void Tof::read() {
   for (int i = 0, i < 4, i++) {
       sensors[i].rangingTest(&measures[i], false);
       
       if ()
   }
}

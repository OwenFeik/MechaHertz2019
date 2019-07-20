// #include "colour.h"
// #include "compass.h"
// #include "drive.h"
// #include "pixy.h"
// #include "toggle.h"
#include "tof.h"

int tof_shutdown_pins[] = {30, 31, 32, 33};

// Colour colour = Colour();
// Pixy pixy = Pixy();
// Compass compass = Compass();
// Drive drive = Drive(8, 9, 10, 11, 13, 12);
// Toggle toggle = Toggle(22, 23);
Tof tof = Tof(tof_shutdown_pins);

void setup() {
    Serial.begin(115200);
    tof.init();
}

void loop() {
    tof.read();
    for (int i = 0; i < 4; i++) {
        Serial.print(tof.readings[i]);
        Serial.print(" ");
    }
    Serial.println();

    // int state = toggle.getState();
    // if (state == 1) {

    // }
    // else if (state == 2) {
    
    // }
    // else {

    // }
}

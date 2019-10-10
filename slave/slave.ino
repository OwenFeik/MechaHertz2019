#include "toggle.h"
#include "tof.h"
#include "gyro.h"

int tof_shutdown_pins[] = {30, 31, 32, 33}; // Front, Left, Right, Back
Toggle toggle = Toggle(24, 25);

int state = 0; // Toggle switch state

void update_all() {
    state = toggle.getState();
    tof.update();
    gyro.update();
}

void setup() {
    update_all();
    Serial.begin(115200);
    gyro.init();
    tof.init();
}

void loop() {
    update_all();
    Serial.write("f");
    Serial.write(tof.front);
    Serial.write("l");
    Serial.write(tof.left);
    Serial.write("b");
    Serial.write(tof.back);
    Serial.write("r");
    Serial.write(tof.right);

    Serial.write("g");
    Serial.write(gyro.heading);
}
}

#include "tof.h"
#include "gyro.h"

int tof_shutdown_pins[] = {30, 31, 32, 33}; // Front, Left, Right, Back
Tof tof = Tof(tof_shutdown_pins);
Gyro gyro = Gyro();

void update_all() {
    tof.update();
    gyro.update();
}

void setup() {
    Serial.begin(115200);
    tof.init();
    gyro.init();
}

void loop() {
    update_all();

    Serial.write('s');
    Serial.write(tof.front);
    Serial.write('f');
    Serial.write(tof.left);
    Serial.write('l');
    Serial.write(tof.right);
    Serial.write('r');
    Serial.write(tof.back);
    Serial.write('b');

    Serial.write((int) gyro.heading);
    Serial.write('g');
}

#include "colour.h"
#include "compass.h"
#include "drive.h"
#include "pixy.h"
#include "toggle.h"

Colour colour = Colour();
Pixy pixy = Pixy();
Compass compass = Compass();
Drive drive = Drive(11, 12, 13, 14, 15, 16);
Toggle toggle = Toggle(22, 23);

void setup() {
    Serial.begin(115200);
    compass.calibrateSelf(); //Doesn't currently work
}

void loop() {
    if (toggle.getState() == 1) {

    }
}




#include "colour.h"
#include "compass.h"
#include "drive.h"
#include "pixy.h"
#include "toggle.h"

Colour colour = Colour();
Pixy pixy = Pixy();
Compass compass = Compass();
Drive drive = Drive(8, 9, 10, 11, 13, 12);
Toggle toggle = Toggle(22, 23);

void setup() {
    Serial.begin(115200);
}

void loop() {
    int state = toggle.getState();
    if (state == 1) {

    }
    else if (state == 2) {
    
    }
    else {

    }
}

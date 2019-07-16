#include "colour.h"
#include "Adafruit_TCS34725.h"

Colour::Colour() {
    sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);
}

//Returns 0 if the current colour below the bot is green, 1 for white and 2 for black
int Colour::getColour() {
    sensor.getRGB(&red, &green, &blue);

    return 0;
}

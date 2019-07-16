#include <Wire.h>
#include "Adafruit_TCS34725.h" //https://github.com/adafruit/Adafruit_TCS34725 // Colour Sensor

class Colour {
    public:
        Adafruit_TCS34725 sensor;
        float red, green, blue;

    Colour();
    int getColour();
};

#include <Wire.h>
#include "Adafruit_TCS34725.h" //https://github.com/adafruit/Adafruit_TCS34725 // Colour Sensor

class Colour {
    public:
        Adafruit_TCS34725 sensor;
        float red, green, blue;
        int g_r_min = 75, g_r_max = 95, g_g_min = 95, g_g_max = 115, g_b_min = 50, g_b_max = 60;
        int w_r_min = 80, w_r_max = 90, w_g_min = 80, w_g_max = 90, w_b_min = 65, w_b_max = 80;
        int b_r_min = 105, b_r_max = 115, b_g_min = 70, b_g_max = 85, b_b_min = 55, b_b_max = 65; 

    Colour();
    void update();
    void print_colour();
    int getColour();
    void setRange(int colour = -1, bool print_to_serial = false);
};

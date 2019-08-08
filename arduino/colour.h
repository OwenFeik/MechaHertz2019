#include <Wire.h>
#include "Adafruit_TCS34725.h" //https://github.com/adafruit/Adafruit_TCS34725 // Colour Sensor

class Colour {
    public:
        Adafruit_TCS34725 sensor;
        float red, green, blue;
        int g_r_min = 0, g_r_max = 20, g_g_min = 260, g_g_max = 280, g_b_min = 175, g_b_max = 195;
        int w_r_min, w_r_max, w_g_min, w_g_max, w_b_min, w_b_max;
        int b_r_min, b_r_max, b_g_min, b_g_max, b_b_min, b_b_max; 

    Colour();
    void update();
    int getColour();
    void setRange(int colour = -1, bool print_to_serial = false);
};

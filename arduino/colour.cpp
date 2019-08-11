#include "colour.h"
#include "Adafruit_TCS34725.h"
#include <Arduino.h>

Colour::Colour() {
    sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);
}


void Colour::update() {
    sensor.getRGB(&red, &green, &blue);
}

//Returns 0 if the current colour below the bot is green, 1 for white and 2 for black
int Colour::getColour() {
    update();

    if (g_r_min < red && g_r_max > red && g_b_min < green && g_g_max > green && g_g_min < blue && g_b_max > blue) {
        return 0;
    }
    else if (w_r_min < red && w_r_max > red && w_b_min < green && w_g_max > green && w_g_min < blue && w_b_max > blue) {
        return 1;
    }
    else if (b_r_min < red && b_r_max > red && b_b_min < green && b_g_max > green && b_g_min < blue && b_b_max > blue) {
        return 2;
    }
    else {
        return -1;
    }
}

/*
    Calibrates the sensor for the colour it is currently seeing.
    Input: The colour to set ranges for.
        0: Green, 1: White, 2: Black
*/
void Colour::setRange(int colour, bool print_to_serial) {
    int r_min = 255, r_max = 0, g_min = 255, g_max = 0, b_min = 255, b_max = 0;

    for (int i = 0; i < 100; i++) {
        sensor.getRGB(&red, &green, &blue);

        if (r_min > red) { r_min = red; }
        if (r_max < red) { r_max = red; }
        if (g_min > green) { g_min = green; }
        if (g_max < green) { g_max = green; }
        if (b_min > blue) { b_min = blue; }
        if (b_max < blue) { b_max = blue; }
    }

    if (colour == 0) {
        g_r_min = r_min;
        g_r_max = r_max;
        g_g_min = g_min;
        g_g_max = g_max;
        g_b_min = b_min;
        g_b_max = b_max;
    }
    else if (colour == 1) {
        w_r_min = r_min;
        w_r_max = r_max;
        w_g_min = g_min;
        w_g_max = g_max;
        w_b_min = b_min;
        w_b_max = b_max;
    }
    else if (colour == 2) {
        b_r_min = r_min;
        b_r_max = r_max;
        b_g_min = g_min;
        b_g_max = g_max;
        b_b_min = b_min;
        b_b_max = b_max;
    }

    if (print_to_serial) {
        Serial.print("r_min: "); Serial.print(r_min);
        Serial.print(" r_max: "); Serial.print(r_max);
        Serial.print(" g_min: "); Serial.print(g_min);
        Serial.print(" g_max: "); Serial.print(g_max);
        Serial.print(" b_min: "); Serial.print(b_min);
        Serial.print(" b_max: "); Serial.println(b_max);
    }
}

void Colour::print_colour() {
    Serial.print("R: ");
    Serial.print(red);
    Serial.print(" G: ");
    Serial.print(green);
    Serial.print(" B: ");
    Serial.print(blue);
    Serial.println();
}

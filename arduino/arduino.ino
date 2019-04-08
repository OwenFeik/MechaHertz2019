#include "compass.h"
#include "drive.h"
#include "pixy.h"

int btn_val; // Digital output of the button
int btn_pin = 22; // Pin the button is wired to
bool go = false; // Stop/go toggled by the button

Compass *compass;
Pixy *pixy;

void setup() {
    Serial.begin(115200);

    compass = new Compass();
    pixy = new Pixy();

    pinMode(btn_pin, INPUT); // Set up the button
}

void loop() {
    btn_val = digitalRead(btn_pin); // Output of the button
    if (btn_val == LOW){ // Button outputs digital LOW when pressed
        go = !go;
        delay(500); // Delay to allow person to remove finger from button
    }

    if (go){
        pixy->update();
        if (pixy->visible){
            Serial.print("X: ");
            Serial.print(pixy->x);
            Serial.print(" Y: ");
            Serial.print(pixy->y);
        }

        compass->getHeading(); // Function that updates the heading
        Serial.print(" H: ");
        Serial.println(compass->heading);
    }
}




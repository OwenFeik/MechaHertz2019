#include "compass.h"
#include "drive.h"

float initial_heading; // Bearing relative to T bot faced initially
float heading; // Current bearing relative to T

int btn_val; // Digital output of the button
int btn_pin = 22; // Pin the button is wired to
bool go = false; // Stop/go toggled by the button
Compass *compass;

void setup() {
    Drive drive(1,2,3);
    Serial.begin(115200);

    compass = new Compass();
    initial_heading=compass->getAccurateHeading(); // Use an accurate reading for baseline
    
    pinMode(btn_pin,INPUT); // Set up the button
}

void loop() {
    btn_val=digitalRead(btn_pin); // Output of the button
    if (btn_val==LOW){ // Button outputs digital LOW when pressed
        go = !go;
        delay(500); // Delay to allow person to remove finger from button
    }

    if (go){
        heading=compass->getHeading(); // Function that updates the heading
        Serial.print("Heading: ");
        Serial.println(heading);
    }
}




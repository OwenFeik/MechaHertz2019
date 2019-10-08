#include "compass.h"
#include "arduino.h"

// Constructor; sets up the compass
Compass::Compass() {
}

void Compass::init() {
    Wire.begin();
    Wire.beginTransmission(0x1E);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();

    initial_heading = getAccurateHeading();
}

// Calculate the x and y offset of the compass (doesn't work atm)
void Compass::calibrateSelf() {
    int min_x = x, max_x = x;
    int min_y = y, max_y = y;
    int min_z = z, max_z = z;

    for (int i = 0; i < 100; i++) {
        updateXYZ();
        if (x > max_x) {max_x = x;}
        if (x < min_x) {min_x = x;}
        if (y > max_y) {max_y = y;}
        if (y < min_y) {min_x = y;}
        if (z > max_z) {max_z = z;}
        if (z < min_z) {min_z = z;}

        delay(100);
    }

    x_bias = int((min_x + max_x) / 2);
    y_bias = int((min_y + max_y) / 2);
    z_bias = int((min_z + max_z) / 2);

    // Serial.print("x bias: ");
    // Serial.println(this->x_bias);
    // Serial.print("y bias: ");
    // Serial.println(this->y_bias);
    // Serial.print("z bias: ");
    // Serial.println(this->z_bias);

    /*
    Scale factor?
    500 / (max - min)?
    */
}

// Update the x, y and z values of the compass object 
void Compass::updateXYZ() {
    Wire.beginTransmission(0x1E);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();

    Wire.requestFrom(0x1E, 6);
    if(6 <= Wire.available()) {
        x = Wire.read() << 8 | Wire.read();
        z = Wire.read() << 8 | Wire.read();
        y = Wire.read() << 8 | Wire.read();
    }
}

// Get current bearing of compass relative to T
void Compass::update() {  
    updateXYZ();

    heading = atan2(x, y)/DEG_TO_RAD; 
    if (heading < 0) {
        heading += 360;
    }
    heading = abs(heading - 360);

    facing_goal = false;
    if (abs(heading - initial_heading) <= 30) {
        facing_goal = true;
    }
}

// Return the average of 10 compass readings
float Compass::getAccurateHeading() {
    float heading_sum = 0;
    for(int i = 0; i < 10; i++) {
        update();
        heading_sum += heading;
    }
    heading = heading_sum / 10;

    return heading;
}

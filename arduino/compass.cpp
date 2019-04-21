#include "compass.h"
#include "arduino.h"

// Constructor; sets up the compass
Compass::Compass() {
    Wire.begin();
    Wire.beginTransmission(0x1E);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();

    this->initial_heading = this->getAccurateHeading();
}

// Calculate the x and y offset of the compass (doesn't work atm)
void Compass::calibrateSelf() {
    int min_x = this->x, max_x = this->x;
    int min_y = this->y, max_y = this->y;
    int min_z = this->z, max_z = this->z;

    for (int i = 0; i < 100; i++) {
        this->updateXYZ();
        if (this->x > max_x) {max_x = this->x;}
        if (this->x < min_x) {min_x = this->x;}
        if (this->y > max_y) {max_y = this->y;}
        if (this->y < min_y) {min_x = this->y;}
        if (this->z > max_z) {max_z = this->z;}
        if (this->z < min_z) {min_z = this->z;}

        delay(100);
    }

    this->x_bias = int((min_x + max_x) / 2);
    this->y_bias = int((min_y + max_y) / 2);
    this->z_bias = int((min_z + max_z) / 2);

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
        this->x = Wire.read() << 8 | Wire.read();
        this->z = Wire.read() << 8 | Wire.read();
        this->y = Wire.read() << 8 | Wire.read();
    }
}

// Get current bearing of compass relative to T
void Compass::update() {  
    this->updateXYZ();

    float heading;
    heading = atan2(this->x, this->y)/0.0174532925;
    if (heading < 0) {
        heading += 360;
    }
    this->heading = abs(heading - 360);

    this->facing_goal = false;
    if (abs(heading - this->initial_heading) <= 30) {
        this->facing_goal = true;
    }
}

// Return the average of 10 compass readings
float Compass::getAccurateHeading() {
    float heading_sum=0;
    for(int i = 0; i < 10; i++) {
        heading_sum += getHeading();
    }
    this->heading = heading_sum/10;

    return this->heading;
}

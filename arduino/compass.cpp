#include "compass.h"
#include "arduino.h"

// Constructor; sets up the compass
Compass::Compass(){
    Wire.begin();
    Wire.beginTransmission(0x1E);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
}

// Calculate the x and y offset of the compass (doesn't work atm)
void Compass::calibrateSelf(){
    int min_x=this->x,min_y=this->y,max_x=this->x,max_y=this->y;

    for (int i=0;i<100;i++){
        this->updateXYZ();
        if (this->x>max_x){max_x=this->x;}
        if (this->y>max_y){max_y=this->y;}
        if (this->x<min_x){min_x=this->x;}
        if (this->y<min_x){min_x=this->y;}
        delay(100);
    }

    this->x_offset=(min_x+max_x)/2;
    this->y_offset=(min_y+max_y)/2;

    Serial.println(x_offset);
    Serial.println(y_offset);
}


void Compass::updateXYZ(){
    Wire.beginTransmission(0x1E);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();

    Wire.requestFrom(0x1E, 6);
    if(6<=Wire.available()){
        this->x = (Wire.read()<<8 | Wire.read());
        this->z = Wire.read()<<8 | Wire.read();
        this->y = (Wire.read()<<8 | Wire.read());
    }
}

// Get current bearing of compass relative to T
float Compass::getHeading(){  

    this->updateXYZ();

    float heading;

    heading=atan2(this->x, this->y)/0.0174532925;
    if (heading<0){
        heading+=360;
    }
    this->heading=abs(heading-360);

    return this->heading;
}

// Return the average of 10 compass readings
float Compass::getAccurateHeading(){
    float heading_sum=0;
    for(int i=0; i<10; i++){
        heading_sum+=getHeading();
    }
    this->heading=heading_sum/10;

    return this->heading;
}

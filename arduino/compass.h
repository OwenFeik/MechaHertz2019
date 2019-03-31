#include <Wire.h> // Library to interface with the magnetometer

//Open communication with compass
void setupCompass(){
    Wire.begin();
    Wire.beginTransmission(0x1E); // Set up the magnetometer
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
}

// Get current bearing of compass relative to T
float getHeading(){  
    Wire.beginTransmission(0x1E);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();

    int x,y,z;

    Wire.requestFrom(0x1E, 6);
    if(6<=Wire.available()){
        x = Wire.read()<<8; //X msb
        x |= Wire.read(); //X lsb
        z = Wire.read()<<8; //Z msb
        z |= Wire.read(); //Z lsb
        y = Wire.read()<<8; //Y msb
        y |= Wire.read(); //Y lsb
    }

    float heading;

    heading=atan2(x, y)/0.0174532925;
    if (heading<0){
        heading+=360;
    }
    heading=abs(heading-360);
    return heading;
}

// Return the average of 10 compass readings
float getAccurateHeading(){
    float heading_sum=0;
    for(int i=0; i<10; i++){
        heading_sum+=getHeading();
    }
    return heading_sum/10;
}

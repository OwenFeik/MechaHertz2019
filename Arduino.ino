#include <Wire.h> // Library to interface with the magnetometer

#define address 0x1E // Address sent to the magnetometer


float heading; // Current bearing of the bot from T

int btn_val; // Digital output of the button
int btn_pin = 22; // Pin the button is wired to

bool go = false; // Stop/go toggled by the button

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(address); // Set up the magnetometer
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();

  pinMode(btn_pin,INPUT); // Set up the button
}

void loop() {
  updateHeading(); // Function that updates the heading

  btn_val=digitalRead(btn_pin); // Output of the button
  if (btn_val==LOW){ // Button outputs digital LOW when pressed
    go = !go;
    delay(500); // Delay to allow person to remove finger from button
  }
 
}

void updateHeading(){  
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

  int x,y,z;

  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  heading=atan2(x, y)/0.0174532925;
  if (heading<0){
    heading+=360;
  }
  heading=abs(heading-360);
}



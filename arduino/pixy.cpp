#include "pixy.h"
#include <Arduino.h> // 'map' function

void Pixy::update(){
    this->pixy.ccc.getBlocks();

    if (this->pixy.ccc.numBlocks){
        int x = this->pixy.ccc.blocks[0].m_x; // Raw x value is between 0 and 316
        int y = this->pixy.ccc.blocks[0].m_y; // Y is in [0,208]

        int heading = atan2(x, y)/0.0174532925; // 0.017 = pi/180, conversion from radians
        if (heading < 0){
            heading += 360; // Change from -180,180 to 0,360
        }
        heading = abs(heading - 360);
        
        this->x = map(x, 0, 316, 0, 100); // Remap to percentage
        this->y = map(y, 0, 208, 0, 100); // Remap to percentage
        this->width = map(this->pixy.ccc.blocks[0].m_width, 1, 316, 1, 100); // Scale to percentage
        this->height = map(this->pixy.ccc.blocks[0].m_height, 1, 208, 1, 100); // Scale to percentage
        this->age = this->pixy.ccc.blocks[0].m_age; // Number of frames object has been visible
        this->heading = heading; // Bearing of the ball from the bot
        this->dist = sqrt(pow(x, 2) + pow(y, 2)); // Pythagorean distance
        this->visible = true; // If the pixy outputs blocks, we are seeing the ball
    }
    else {
        this->visible = false; // If it didn't output blocks, we didn't see the ball
    }
}

Pixy::Pixy(){
    this->pixy.init(); // Needed to start up the PixyCam
    this->visible = false; // Start the ball as unseen
}

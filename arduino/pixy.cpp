#include "pixy.h"
#include <Arduino.h> // 'map' function

Pixy::Pixy() {
    pixy.init(); // Needed to start up the PixyCam
    visible = false; // Start the ball as unseen
}

void Pixy::update() {
    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) {
        x = pixy.ccc.blocks[0].m_x; // Raw x value is between 0 and 316
        y = pixy.ccc.blocks[0].m_y; // Y is in [0,208]

        // Robot is the origin, so subtract half of the frame size from x and y
        heading = atan2(x - 158, y - 104)/0.0174532925; // 0.017 = pi/180, conversion from radians
        if (heading < 0) {
            heading += 360; // Change from -180,180 to 0,360
        }
        heading = abs(heading - 360);
        
        bool in_front = false;
        if ((heading >= 0 && heading <= 30) || (heading <= 360 && heading >= 330)) {
            in_front = true;
        }

        x = map(x, 0, 316, 0, 100); // Remap to percentage
        y = map(y, 0, 208, 0, 100); // Remap to percentage
        width = map(pixy.ccc.blocks[0].m_width, 1, 316, 1, 100); // Scale to percentage
        height = map(pixy.ccc.blocks[0].m_height, 1, 208, 1, 100); // Scale to percentage
        age = pixy.ccc.blocks[0].m_age; // Number of frames object has been visible
        heading = heading; // Bearing of the ball from the bot
        dist = sqrt(pow(x, 2) + pow(y, 2)); // Pythagorean distance
        visible = true; // If the pixy outputs blocks, we are seeing the ball
        in_front = in_front; // Is the ball in a 60 degree cone ahead of us?
    }
    else {
        visible = false; // If it didn't output blocks, we didn't see the ball
    }
}

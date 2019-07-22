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

        x = map(x, 0, 316, 0, 100); // Remap to percentage
        y = map(y, 0, 208, 0, 100); // Remap to percentage

        in_front = false;
        if (x > 25 && x < 75) {
            in_front = true;
        }
        
        width = map(pixy.ccc.blocks[0].m_width, 1, 316, 1, 100); // Scale to percentage
        height = map(pixy.ccc.blocks[0].m_height, 1, 208, 1, 100); // Scale to percentage
        age = pixy.ccc.blocks[0].m_age; // Number of frames object has been visible
        visible = true; // If the pixy outputs blocks, we are seeing the ball
        last_seen = 0; // Frames since we spotted the ball
    }
    else {
        in_front = false;
        visible = false; // If it didn't output blocks, we didn't see the ball
        last_seen += 1;
    }
}

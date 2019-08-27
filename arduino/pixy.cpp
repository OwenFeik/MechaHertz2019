#include "pixy.h"
#include <Arduino.h> // 'map' function

#define BALL_SIGNATURE 1
#define BLUE_SIGNATURE 2
#define YELLOW_SIGNATURE 3

Pixy::Pixy() {
    pixy.init(); // Needed to start up the PixyCam
    visible = false; // Start the ball as unseen
}

void Pixy::update() {
    pixy.ccc.getBlocks();

    last_seen += 1;
    u_last_seen += 1;
    y_last_seen += 1;

    visible = false;
    u_visible = false;
    y_visible = false;

    if (pixy.ccc.numBlocks) {
        
        for (int i = 0; i < pixy.ccc.numBlocks; i++) {

            int _x = map(pixy.ccc.blocks[i].m_x, 0, 316, 0, 100); // Raw x value is between 0 and 316
            int _y = 100 - map(pixy.ccc.blocks[i].m_y, 0, 208, 0, 100); // Y is in [0,208], subtract from 100 to reverse direciton
            int _w = pixy.ccc.blocks[i].m_width;
            int _h = pixy.ccc.blocks[i].m_height; // Scale to percentage
            int _age = pixy.ccc.blocks[i].m_age; // Number of frames object has been visible

            int signature = pixy.ccc.blocks[i].m_signature; 
            if (signature == BALL_SIGNATURE) {
                x = _x;
                y = _y;
                width = _w;
                height = _h;
                age = _age;
                last_seen = 0;
                visible = true;                
            }
            else if (signature == BLUE_SIGNATURE) {
                u_x = _x;
                u_y = _y;
                u_w = _w;
                u_h = _h;
                u_age = _age;
                u_last_seen = 0;
                u_visible = true;                
            }
            else if (signature == YELLOW_SIGNATURE) {
                y_x = _x;
                y_y = _y;
                y_w = _w;
                y_h = _h;
                y_age = _age;
                y_last_seen = 0;
                y_visible = true;                
            }
        }
    }
}

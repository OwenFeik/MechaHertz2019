#include "pixy.h"
#include <Arduino.h> //map function

void Pixy::update(){
    this->pixy.ccc.getBlocks();

    if (this->pixy.ccc.numBlocks){
        this->x=map(this->pixy.ccc.blocks[0].m_x,0,316,0,100);
        this->y=map(this->pixy.ccc.blocks[0].m_y,0,208,0,100);
        this->width=map(this->pixy.ccc.blocks[0].m_width,1,316,1,100);
        this->height=map(this->pixy.ccc.blocks[0].m_height,1,208,1,100);
        this->age=this->pixy.ccc.blocks[0].m_age;
        this->visible=true;
    }
    else{
        this->visible=false;
    }
}

Pixy::Pixy(){
    this->pixy.init();
    this->visible=false;
}

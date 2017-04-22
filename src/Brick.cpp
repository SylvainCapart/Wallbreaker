/* 
 * File:   Brick.cpp
 * Author: Sylvain
 * 
 * Created on 29 janvier 2017, 10:22
 */

#include "Brick.h"
#include "Const.h"

Brick::Brick(int x, int y, int resistance) {
    this->x = x;
    this->y = y;
    this->w = BRICK_WIDTH;
    this->h = BRICK_HEIGHT;
    this->resistance = resistance;
    
}




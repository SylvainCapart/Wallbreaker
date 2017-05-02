/* 
 * File:   Brick.cpp
 * Author: Sylvain
 * 
 * Created on 29 janvier 2017, 10:22
 */

#include "Brick.h"
#include "Const.h"

Brick::Brick(int x, int y, int resistance) {
    m_x = x;
    m_y = y;
    m_w = BRICK_WIDTH;
    m_h = BRICK_HEIGHT;
    m_resistance = resistance;
    
}




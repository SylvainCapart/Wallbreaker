/* 
 * File:   Racket.cpp
 * Author: Sylvain
 * 
 * Created on 5 janvier 2017, 10:34
 */

#include "Racket.h"

using namespace std;

Racket::Racket(int x, int y, ORIENTATION direction) {
    m_x = x;
    m_y = y;

    m_direction = direction;
    if (HORIZONTAL == direction) {
        m_w = RACKET_WIDTH;
        m_h = RACKET_HEIGHT;

    } else {
        m_w = RACKET_HEIGHT;
        m_h = RACKET_WIDTH;

    } 
    m_racketCenter.x = this->m_x + this->m_w / 2;
    m_racketCenter.y = this->m_y + this->m_h / 2;
    
    m_racketSpeed = RACKET_SPEED;
}



void Racket::moveRight() {
    m_x = m_x + m_racketSpeed;
    if (m_x > WINDOW_WIDTH - RACKET_WIDTH)
        m_x = WINDOW_WIDTH - RACKET_WIDTH;

    m_racketCenter.x = m_x + m_w / 2;
    m_racketCenter.y = m_y + m_h / 2;
    accelerate();
}

void Racket::moveLeft() {
    m_x = m_x - m_racketSpeed;
    if (m_x < 0)
        m_x = 0;
    m_racketCenter.x = m_x + m_w / 2;
    m_racketCenter.y = m_y + m_h / 2;
    accelerate();
}

void Racket::moveDown() {
    m_y = m_y + m_racketSpeed;
    if (m_y > WINDOW_HEIGHT - RACKET_WIDTH)
        m_y = WINDOW_HEIGHT - RACKET_WIDTH;
    m_racketCenter.x = m_x + m_w / 2;
    m_racketCenter.y = m_y + m_h / 2;
    accelerate();
}

void Racket::moveUp() {
    m_y = m_y - m_racketSpeed;
    if (m_y < 0)
        m_y = 0;
    m_racketCenter.x = m_x + m_w / 2;
    m_racketCenter.y = m_y + m_h / 2;
    accelerate();
}

/* Function : accelerate
 * Input : none
 * Output : none
 * Goal : allows the racket to accelerate up to a maximum speed
 * 
 */

void Racket::accelerate() {
    m_racketSpeed = m_racketSpeed + RACKET_ACC;
    if (m_racketSpeed > MAX_RACKET_SPEED)
        m_racketSpeed = MAX_RACKET_SPEED;
}

/* Function : accelerate
 * Input : none
 * Output : none
 * Goal : allows the racket to decrease its speed instead of stopping immediatly.
 */

void Racket::decelerate() {
    while (m_racketSpeed > MIN_RACKET_SPEED) {
        m_racketSpeed = m_racketSpeed - RACKET_DEC;
        if (m_racketSpeed < MIN_RACKET_SPEED)
            m_racketSpeed = MIN_RACKET_SPEED;
    }
}

Point Racket::getRacketCenter() const {
    Point center;
    center.x = m_x + m_w/2;
    center.y = m_y + m_h/2;
    return center;
}

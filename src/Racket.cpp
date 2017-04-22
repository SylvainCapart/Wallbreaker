/* 
 * File:   Racket.cpp
 * Author: Sylvain
 * 
 * Created on 5 janvier 2017, 10:34
 */

#include "Racket.h"

using namespace std;

Racket::Racket(int x, int y, ORIENTATION direction) {
    this->x = x;
    this->y = y;

    this->direction = direction;
    if (direction == HORIZONTAL) {
        this->w = RACKET_WIDTH;
        this->h = RACKET_HEIGHT;

    } else if (direction == VERTICAL) {
        this->w = RACKET_HEIGHT;
        this->h = RACKET_WIDTH;

    }
    this->racketCenter.x = this->x + this->w / 2;
    this->racketCenter.y = this->y + this->h / 2;
    
    this->racketSpeed = RACKET_SPEED;
}



void Racket::moveRight() {
    x = x + racketSpeed;
    if (x > WINDOW_WIDTH - RACKET_WIDTH)
        x = WINDOW_WIDTH - RACKET_WIDTH;

    racketCenter.x = x + w / 2;
    racketCenter.y = y + h / 2;
    accelerate();
}

void Racket::moveLeft() {
    x = x - racketSpeed;
    if (x < 0)
        x = 0;
    racketCenter.x = x + w / 2;
    racketCenter.y = y + h / 2;
    accelerate();
}

void Racket::moveDown() {
    y = y + racketSpeed;
    if (y > WINDOW_HEIGHT - RACKET_WIDTH)
        y = WINDOW_HEIGHT - RACKET_WIDTH;
    racketCenter.x = x + w / 2;
    racketCenter.y = y + h / 2;
    accelerate();
}

void Racket::moveUp() {
    y = y - racketSpeed;
    if (y < 0)
        y = 0;
    racketCenter.x = x + w / 2;
    racketCenter.y = y + h / 2;
    accelerate();
}

/* Function : accelerate
 * Input : none
 * Output : none
 * Goal : allows the racket to accelerate until a maximum speed
 * 
 */

void Racket::accelerate() {
    racketSpeed = racketSpeed + RACKET_ACC;
    if (racketSpeed > MAX_RACKET_SPEED)
        racketSpeed = MAX_RACKET_SPEED;
}

/* Function : accelerate
 * Input : none
 * Output : none
 * Goal : allows the racket to decrease its speed instead of stopping immediatly.
 */

void Racket::decelerate() {
    while (racketSpeed > MIN_RACKET_SPEED) {
        racketSpeed = racketSpeed - RACKET_DEC;
        if (racketSpeed < MIN_RACKET_SPEED)
            racketSpeed = MIN_RACKET_SPEED;
    }
}

Point Racket::getRacketCenter() const {
    Point center;
    center.x = x + w/2;
    center.y = y + h/2;
    return center;
}

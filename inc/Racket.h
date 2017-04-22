/* 
 * File:   Racket.h
 * Author: Sylvain
 *
 * Created on 5 janvier 2017, 10:34
 */

#ifndef RACKET_H
#define	RACKET_H

#include "Const.h"
#include "Types.h"
#include <iostream>

class Racket {
public:

    Racket(int x, int y, ORIENTATION direction);

    int getX() {return this->x;}
    int getY() {return this->y;}
    ORIENTATION getOrientation() {return this->direction;}
    void setX(int x) {this->x = x;}
    void setY(int y) {this->y = y;}
    
    void accelerate();
    void decelerate();
    
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();
    
    ~Racket() {};

    int getW() const {return w;}
    int getH() const {return h;}

    Point getRacketCenter() const;
    float getRacketSpeed() const {return racketSpeed;}
    
private:
    int x, y, h, w;
    float racketSpeed;
    Point racketCenter;
    ORIENTATION direction;

};


#endif	/* RACKET_H */


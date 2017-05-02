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

    /// \param x : x origin of the rectangle SDL_Rect of the racket
    /// \param y : y origin of the rectangle SDL_Rect of the racket
    /// \param direction : horizontal or vertical
    Racket(int x, int y, ORIENTATION direction);

    int getX() {return this->m_x;}
    int getY() {return this->m_y;}
    
    ORIENTATION getOrientation() {return this->m_direction;}
    void setX(int x) {this->m_x = x;}
    void setY(int y) {this->m_y = y;}
    
    /// \goal : when the player moves the racket, it gives to it a bit of acceleration, up to a maximum
    void accelerate();
    
    /// \goal : when the player does not move the racket, decrease its speed to 0.
    void decelerate();
    
    /// \goal : moves the rackets to the right direction
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();
    
    ~Racket() {};

    int getW() const {return m_w;}
    int getH() const {return m_h;}

    Point getRacketCenter() const;
    float getRacketSpeed() const {return m_racketSpeed;}
    
private:
    int m_x, m_y, m_h, m_w; // caracteristics of the rectangle SDL_Rect corresponding to the racket
    float m_racketSpeed; // racket speed
    Point m_racketCenter; // racket center
    ORIENTATION m_direction; //direction : horizontal or vertical

};

#endif	/* RACKET_H */


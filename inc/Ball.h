/* 
 * File:   Ball.h
 * Author: Sylvain
 *
 * Created on 6 janvier 2017, 18:46
 */

#ifndef BALL_H
#define	BALL_H

#include <string>
#include <SDL.h>
#include "Const.h"
#include "Types.h"
#include <cmath>
#include <iostream>



class Ball {
public:
    Ball();
    virtual ~Ball();
    
    void setH(int h) {this->h = h;}
    int getH() const {return h;}

    void setW(int w) {this->w = w;}
    int getW() const {return w;}

    void setLaunched(bool launched) {this->launched = launched;}
    bool isLaunched() const {return launched;}

    void setPlaced(bool placed) {this->placed = placed;}
    bool isPlaced() const {return placed;}

    void setY(float y) {this->y = y;}
    float getY() const {return y;}

    void setX(float x) {this->x = x;}
    float getX() const {return x;}
    
    void setInitialRacketId(int initialRacketId) {this->initialRacketId = BOTTOM;}
    SegmentPoints getBrickSegment(const SDL_Rect& obstacle, OBSTACLE_SEGMENT side);

    void launch();
    void move();
    void setInitialSpeed(int racketId);

    float getY_speed() const {return y_speed;}
    float getX_speed() const {return x_speed;}
    
    void setY_speed(float y_speed);
    void setX_speed(float x_speed);
    
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();
    
    SegmentPoints rebound(SDL_Rect obstacle);

    float getYCenter() const {return yCenter;}
    float getXCenter() const {return xCenter;}

    
private:
    //SDL_Rect m_drawArea;
    float x; //X origin of the ball drawing (same origin as the corresponding SDL_Rect)
    float y; //Y origin of the ball drawing (same origin as the corresponding SDL_Rect)
    int w; // Width of the drawing area
    int h; // Height of the drawing area
    
    float xCenter; // X center component 
    float yCenter; // Y center component
    int r;         // ball radius
    
    /* TODO : improvement 
     * Having components linked to the drawing rectangle could be avoided in the ball class,
     * keeping only the center and the radius.
     */

    float x_speed; // X speed component
    float y_speed; // Y speed component
    
    bool launched; // ball launched from the racket or not
    bool placed; // ball placed on a racket or not
    int initialRacketId; // racket where the ball shall be placed
    

};


#endif	/* BALL_H */


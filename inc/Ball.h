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
    
    /// \param h : height (int) of the SDL_Rect corresponding to the ball
    void setH(int h) {this->m_h = h;}
    
    /// \return : height (int) of the SDL_Rect corresponding to the ball
    int getH() const {return m_h;}

    /// \param w : width (int) of the SDL_Rect corresponding to the ball
    void setW(int w) {this->m_w = w;}
    
    /// \return : width (int) of the SDL_Rect corresponding to the ball
    int getW() const {return m_w;}
    
    /// \param launched : setting (boolean) if the ball is launched (it moves, and not on a racket) or not (it doesn't move, still on a racket)
    void setLaunched(bool launched) {this->m_launched = launched;}
    
    /// \return : boolean, true if the ball is launched (it moves, and not on a racket) or not (it doesn't move, still on a racket)
    bool isLaunched() const {return m_launched;}

    /// \param placed : setting (boolean) if the ball position is initialized on a racket or not
    void setPlaced(bool placed) {this->m_placed = placed;}
    
    /// \return : boolean, true is the ball is placed on a racket, or false if not
    bool isPlaced() const {return m_placed;}

    /// \param y : setting y (float) coordinate of the ball
    void setY(float y) {this->m_y = y;}
    
    /// \return : y coordinate (float) of the ball
    float getY() const {return m_y;}

    /// \param x : setting x (float) coordinate of the ball
    void setX(float x) {this->m_x = x;}
    
    /// \return : x (float) coordinate of the ball
    float getX() const {return m_x;}
    
    /// \param initialRacketId : int, ID of one of the rackets
    void setInitialRacketId(int initialRacketId) {this->m_initialRacketId = initialRacketId;}

    /// \goal : triggers the launch of the ball while indicating it is not placed anymore on the racket
    void launch();
    
    /// \goal : updates the position of the ball following its speed.
    ///         checks speed limits
    ///         checks position limits and reinitiate the ball if it goes past the borders
    void move();
    
    /// \param racketId : int, ID of a racket
    /// \goal : following the racket, give the ball an orthogonal speed;
    void setInitialSpeed(int racketId);

    /// \return : float, y component of the ball speed
    float getY_speed() const {return m_ySpeed;}
    
    /// \return : float, x component of the ball speed
    float getX_speed() const {return m_xSpeed;}
    
    /// \param y_speed : setting y speed of the ball, float
    void setY_speed(float y_speed);
    
    /// \param x_speed : setting x speed of the ball, float
    void setX_speed(float x_speed);
    
    /// \function : moveRight, moveLeft, moveDown, moveUp
    /// \goal : for the ball to follow the racket when placed and not launched 
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();

    /// \goal : return y coordinate of the ball center 
    float getYCenter() const {return m_yCenter;}
    
    /// \goal : return x coordinate of the ball center 
    float getXCenter() const {return m_xCenter;}

    
private:
    float m_x; //X origin of the ball drawing (same origin as the corresponding SDL_Rect)
    float m_y; //Y origin of the ball drawing (same origin as the corresponding SDL_Rect)
    int m_w; // Width of the drawing area
    int m_h; // Height of the drawing area
    
    float m_xCenter; // X center component 
    float m_yCenter; // Y center component
    int m_r;         // ball radius
    
    /* TODO : improvement 
     * Having components linked to the drawing rectangle could be avoided in the ball class,
     * keeping only the center and the radius.
     */

    float m_xSpeed; // X speed component
    float m_ySpeed; // Y speed component
    
    bool m_launched; // ball launched from the racket or not
    bool m_placed; // ball placed on a racket or not
    int m_initialRacketId; // racket where the ball shall be placed
    

};


#endif	/* BALL_H */


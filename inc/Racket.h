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
#include "IDrawable.h"
#include "IKeyboardListener.h"

class Racket : public IDrawable, public IKeyboardListener{
public:
    /// \param renderer : where to draw the racket
    /// \param x : x origin of the rectangle SDL_Rect of the racket
    /// \param y : y origin of the rectangle SDL_Rect of the racket
    /// \param direction : horizontal or vertical
    Racket(SDL_Renderer * renderer, int x, int y, T_ORIENTATION direction,const Color& fillColor, const Color& borderColor,
            T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition);

    virtual void draw() const;

    int getX() {return this->m_x;}
    int getY() {return this->m_y;}
    
    T_ORIENTATION getOrientation() {return this->m_direction;}
    void setX(int x) {this->m_x = x;}
    void setY(int y) {this->m_y = y;}
    int getW() const {return m_w;}
    int getH() const {return m_h;}
    
    /// \goal : when the player moves the racket, it gives to it a bit of acceleration, up to a maximum
    void accelerate();
    
    /// \goal : when the player does not move the racket, decrease its speed to 0.
    void decelerate();
    
    /// \goal : moves the rackets to the right direction
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();

    virtual void onKeyDown(SDL_Event* evt);
    virtual void onKeyUp(SDL_Event* evt);
    virtual void onKeyPressed(std::map<int, int>& keyMap);


    const Point & getRacketCenter() const;
    float getRacketSpeed() const {return m_racketSpeed;}
    void setFillColor(const Color fillColor);
    const Color getFillColor() const;
    virtual void setMiddleRacketAngle(int middleRacketAngle);
    virtual int getMiddleRacketAngle() const;
    virtual void setMiddleRacketRotationActivated(bool middleRacketRotationActivated);
    virtual bool isMiddleRacketRotationActivated() const;
    virtual void rotate();
    
    SegmentPoints getReboundSurface() const;
    Vector & modifySpeedVector(Vector & speedVector, int speedModifCoeff, float contactRatio, float squareNormSpeed);
    
protected:
    int m_x, m_y, m_h, m_w; // caracteristics of the rectangle SDL_Rect corresponding to the racket
    float m_racketSpeed; // racket speed
    Point m_racketCenter; // racket center
    T_ORIENTATION m_direction; //direction : horizontal or vertical
    SDL_Renderer * m_renderer;
    const Color m_fillColor;
    const Color m_borderColor;
    T_KEY_BIND m_firstBind;
    T_KEY_BIND m_secondBind;
    T_RACKET_POSITION m_racketPosition;

};

#endif	/* RACKET_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RotatingRacket.h
 * Author: user
 *
 * Created on 6 juillet 2017, 18:02
 */

#ifndef ROTATINGRACKET_H
#define ROTATINGRACKET_H

#include "Racket.h"

class RotatingRacket : public Racket {
public:
    RotatingRacket(SDL_Renderer * renderer, int x, int y, const Color& fillColor, const Color& borderColor, T_KEY_BIND firstBind,
            T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition);
    virtual ~RotatingRacket();
    virtual void draw() const;
    
    void setMiddleRacketAngle(int middleRacketAngle) {m_middleRacketAngle = middleRacketAngle;}
    int getMiddleRacketAngle() const {return m_middleRacketAngle;}

    /// \goal : functions that a IKeyboardListener must implement
    virtual void onKeyDown(SDL_Event* evt);
    virtual void onKeyUp(SDL_Event* evt);
    virtual void onKeyPressed(std::map<int, int>& keyMap);

    /// \goal : rotation when the key space is pressed
    virtual void rotate();
    
    virtual void setMiddleRacketRotationActivated(bool middleRacketRotationActivated)
        {m_middleRacketRotationActivated = middleRacketRotationActivated;}
    virtual bool isMiddleRacketRotationActivated() const
        {return m_middleRacketRotationActivated;}

    
private:
    SDL_Texture * m_racketTexture;
    int m_middleRacketAngle; // angle of rotation for the middle racket
    bool m_middleRacketRotationActivated;     

};
#endif /* ROTATINGRACKET_H */


/* 
 * File:   Racket.cpp
 * Author: Sylvain
 * 
 * Created on 5 janvier 2017, 10:34
 */

#include "Racket.h"

using namespace std;

Racket::Racket(SDL_Renderer * renderer, int x, int y, T_ORIENTATION direction, const Color & fillColor,
        const Color & borderColor, T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition):m_x(x), m_y(y), m_direction(direction),
        m_renderer(renderer), m_fillColor(fillColor), m_borderColor(borderColor), m_firstBind(firstBind), m_secondBind(secondBind), m_racketPosition(racketPosition) {
    
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

void Racket::onKeyDown(SDL_Event* evt) {

}

void Racket::onKeyUp(SDL_Event* evt) {
    switch (evt->key.keysym.sym)
    {
        case SDLK_LEFT:
        case SDLK_RIGHT:
            if ( K_LEFT== m_firstBind || K_RIGHT== m_firstBind || K_LEFT== m_secondBind || K_RIGHT== m_secondBind )
                this->decelerate();
            break;
        case SDLK_UP:
        case SDLK_DOWN:
            if ( K_UP== m_firstBind || K_DOWN== m_firstBind || K_UP== m_secondBind || K_DOWN== m_secondBind )
                this->decelerate();            
            break;
        case SDLK_z:
        case SDLK_s:
            if ( K_Z== m_firstBind || K_S== m_firstBind || K_Z== m_secondBind || K_S== m_secondBind )
                this->decelerate();
            break;
        case SDLK_q:
        case SDLK_d:
            if ( K_Q == m_firstBind || K_D == m_firstBind || K_D == m_secondBind || K_D == m_secondBind )
                this->decelerate();            
            break;
        default:
            break;
    }
}

void Racket::onKeyPressed(std::map<int, int>& keyMap) {
 
            if ( keyMap[SDLK_LEFT] && (K_LEFT== m_firstBind || K_LEFT == m_secondBind) )
                moveLeft();
            if ( keyMap[SDLK_RIGHT] && (K_RIGHT== m_firstBind || K_RIGHT == m_secondBind) )
                moveRight();
            if ( keyMap[SDLK_UP] && (K_UP== m_firstBind || K_UP == m_secondBind) )
                moveUp();
            if ( keyMap[SDLK_DOWN] && (K_DOWN== m_firstBind || K_DOWN == m_secondBind) )
                moveDown();
            if ( keyMap[SDLK_s] && (K_S== m_firstBind || K_S == m_secondBind) )
                moveDown();
            if ( keyMap[SDLK_z] && (K_Z== m_firstBind || K_Z == m_secondBind) )
                moveUp();
            if ( keyMap[SDLK_q] && (K_Q== m_firstBind || K_Q == m_secondBind) )
                moveLeft();
            if ( keyMap[SDLK_d] && (K_D== m_firstBind || K_D == m_secondBind) )
                moveRight();
}


void Racket::draw() const {
    SDL_Rect racketRect = {m_x, m_y, m_w, m_h};
    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r,m_borderColor.g,m_borderColor.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_renderer, m_x, m_y, m_x + m_w, m_y);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y, m_x + m_w, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y + m_h, m_x, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x, m_y + m_h, m_x, m_y);
    SDL_SetRenderDrawColor(m_renderer, m_fillColor.r, m_fillColor.g, m_fillColor.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, &racketRect);
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

const Point & Racket::getRacketCenter() const {
    return m_racketCenter;
}

int Racket::getMiddleRacketAngle() const {
    // redefined in derived classes
    return 0;
}

void Racket::setMiddleRacketAngle(int middleRacketAngle) {
    // redefined in derived classes
}

bool Racket::isMiddleRacketRotationActivated() const {
    // redefined in derived classes
    return false;
}

void Racket::setMiddleRacketRotationActivated(bool middleRacketRotationActivated) {
    // redefined in derived classes
}

void Racket::rotate() {
    // redefined in derived classes
}


SegmentPoints Racket::getReboundSurface() const {
    SegmentPoints AB;
    switch(m_racketPosition)
    {
        case BOTTOM:
            AB.A.x = m_x; // AB segment where rebound is done 
            AB.A.y = m_y;
            AB.B.x = m_x + m_w;
            AB.B.y = m_y;
            break;
        case TOP:
            AB.A.x = m_x; // AB segment where rebound is done 
            AB.A.y = m_y + m_h;
            AB.B.x = m_x + m_w;
            AB.B.y = m_y + m_h;
            break;
        case LEFT :
            AB.A.x = m_x + m_w; // AB segment where rebound is done 
            AB.A.y = m_y;
            AB.B.x = m_x + m_w;
            AB.B.y = m_y +m_h;
            break;
        case RIGHT:
            AB.A.x = m_x; // AB segment where rebound is done 
            AB.A.y = m_y;
            AB.B.x = m_x;
            AB.B.y = m_y + m_h;
            break;
        default:
            break;
    }
    return AB;
}

Vector& Racket::modifySpeedVector(Vector& speedVector, int speedModifCoeff, float contactRatio, float squareNormSpeed) {
    switch (m_racketPosition)
    {
        case BOTTOM:
            // setting the new speed. 0.5 because we count from the middle of the racket.
            // if tempratio = 0.5 ( ie hit at the center of the racket ) then 
            // the new speed is not modified and is like a "normal" rebound
            speedVector.x = speedVector.x - speedModifCoeff * (0.5 - contactRatio);
            
            // the other speed component is defined regarding the first one,
            // so that speed norm remains unchanged.
            speedVector.y = -sqrt(fabs(squareNormSpeed - speedVector.x * speedVector.x));
            break;
        case TOP:
            speedVector.x = speedVector.x - speedModifCoeff * (0.5 - contactRatio);
            speedVector.y = sqrt(fabs(squareNormSpeed - speedVector.x * speedVector.x));
            break;
        case LEFT:
            speedVector.y = speedVector.y - speedModifCoeff * (0.5 - contactRatio);
            speedVector.x = sqrt(fabs(squareNormSpeed - speedVector.y * speedVector.y));
            break;
        case RIGHT:
            speedVector.y = speedVector.y - speedModifCoeff * (0.5 - contactRatio);
            speedVector.x = -sqrt(fabs(squareNormSpeed - speedVector.y * speedVector.y));
            break;
        default:
            break;
    }
    return speedVector;
}

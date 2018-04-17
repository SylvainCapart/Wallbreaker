/* 
 * File:   Ball.cpp
 * Author: Sylvain
 * 
 * Created on 6 janvier 2017, 18:46
 */

#include "Ball.h"
#include <string>
#include <SDL.h>
#include "Const.h"
#include "Types.h"
#include <cmath>
#include <iostream>

using namespace std;

Ball::Ball(SDL_Renderer * renderer, const char * textureFileName):m_x(0), m_y(0), m_xCenter(0), m_yCenter(0), m_xSpeed(0), m_ySpeed(0),
m_launched(false), m_placed(false), m_renderer(renderer) {
    m_w = LITTLE_BALL_DIAMETER;
    m_h = LITTLE_BALL_DIAMETER;
    m_r = LITTLE_BALL_DIAMETER / 2;
    m_ballRect.x = m_x;
    m_ballRect.y = m_y;
    m_ballRect.w = m_w;
    m_ballRect.h = m_h;
    m_ballTexture = IMG_LoadTexture(renderer, textureFileName);
    if (NULL == m_ballTexture)
    cerr << "Error charging ball graphics" << endl;
}

Ball::~Ball() {
    SDL_DestroyTexture(m_ballTexture);
}

void Ball::setY_speed(float y_speed) {
 
    m_ySpeed = y_speed;

    // control of y speed component
    if (m_ySpeed > MAX_Y_BALL_SPEED)
        m_ySpeed = MAX_Y_BALL_SPEED;
    if (m_ySpeed < - MAX_Y_BALL_SPEED)
        m_ySpeed = - MAX_Y_BALL_SPEED;
    


}

void Ball::setX_speed(float x_speed) {

    m_xSpeed = x_speed;
    
    // control of x speed component
    if (m_xSpeed > MAX_X_BALL_SPEED)
        m_xSpeed = MAX_X_BALL_SPEED;
    if (m_xSpeed < - MAX_X_BALL_SPEED)
        m_xSpeed = - MAX_X_BALL_SPEED;
    

    
}



/* Function : launch
 * Input : none
 * Output : none
 * Goal : triggers the launch of the ball while indicating it is not placed anymore on the racket
 */

void Ball::launch() {
    m_launched = true;
    m_placed = false;
}

/* Function : move
 * Input : none
 * Output : none
 * Goal : updates the position of the ball following its speed.
 *        checks speed limits
 *        checks position limits and reinitiate the ball if it goes past the borders
 */

void Ball::move() {
    
    if (m_xSpeed > MAX_X_BALL_SPEED)
        m_xSpeed = MAX_X_BALL_SPEED;
    if (m_ySpeed > MAX_Y_BALL_SPEED)
        m_ySpeed = MAX_Y_BALL_SPEED;
        
    m_x = m_x + m_xSpeed;
    m_y = m_y + m_ySpeed;
    m_xCenter = m_x + m_r;
    m_yCenter = m_y + m_r;


    if ((m_x + m_w < 0) || (m_x > WINDOW_WIDTH) || (m_y + m_h < 0) || (m_y > WINDOW_HEIGHT)) {
        m_launched = false;
        m_placed = false;
    }
    
    if(isnan(m_x) || isnan(m_y)){ //should not happen or a bug is present.
        m_launched = false;
        m_placed = false;
    }
}

/* Function : setInitialSpeed
 * Input : int racketId
 * Output : none
 * Goal : following the racket, give the ball an orthogonal speed;
 * 
 */

void Ball::setInitialSpeed() {
    float initialSpeed = INITIAL_SPEED;
    
    switch (m_initialRacketId) {
        case BOTTOM:
            m_xSpeed = 0;
            m_ySpeed = -initialSpeed;
            break;
            
        case TOP:
            m_xSpeed = 0;
            m_ySpeed = initialSpeed;
            break;
            
        case LEFT:
            m_xSpeed = initialSpeed;
            m_ySpeed = 0;
            break;
            
        case RIGHT:
            m_xSpeed = -initialSpeed;
            m_ySpeed = 0;
            break;
        case MIDDLE:
            break;
        default:
            break;
    }
}


void Ball::draw() const {
    SDL_Rect ballRect = {m_x, m_y, m_w, m_h};
    SDL_RenderCopy(m_renderer, m_ballTexture, NULL, &ballRect); // copy the rectangle on the renderer
}

void Ball::update(T_RACKET_POSITION racketPosition, Racket * rackets[TOTAL_RACKET_NUMBER]) {
    if (!this->isLaunched()) {
        switch (racketPosition) {
            case BOTTOM:
                m_x = rackets[BOTTOM]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2;
                break;
            case RIGHT:
                m_y = rackets[RIGHT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2;
                break;
            case LEFT:
                m_y = rackets[LEFT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2;
                break;
            case TOP:
                m_x = rackets[TOP]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2;
                break;
            case MIDDLE: // MIDDLE cannot happen
                break;
            default: 
                break;
        }
    } else
        this->move();
}

/* Function : placeBall
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : place the ball on a random racket
 */

T_RACKET_POSITION Ball::placeBall(Racket * rackets[TOTAL_RACKET_NUMBER]) {

    m_initialRacketId = static_cast<T_RACKET_POSITION>(rand() % BASE_RACKET_NUMBER); //safe here
    this->setInitialRacketId(m_initialRacketId);
    switch (m_initialRacketId) {
        case BOTTOM:
            m_x = rackets[BOTTOM]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2;
            m_y = rackets[BOTTOM]->getY() - LITTLE_BALL_DIAMETER;
            break;
        case TOP:
            m_x = rackets[TOP]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2;
            m_y = rackets[TOP]->getY() + RACKET_HEIGHT;
            break;
        case LEFT:
            m_x = rackets[LEFT]->getX() + RACKET_HEIGHT;
            m_y = rackets[LEFT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2 ;
            break;
        case RIGHT:
            m_x = rackets[RIGHT]->getX() - LITTLE_BALL_DIAMETER;
            m_y = rackets[RIGHT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2;
            break;
        default: // MIDDLE cannot happen
            break;
    }
    
    this->setPlaced(true);
    return m_initialRacketId;
}

void Ball::setBallRect(const SDL_Rect & ballRect) {
    m_ballRect = ballRect;
}

SDL_Rect Ball::getBallRect() const {
    return m_ballRect;
}

void Ball::onKeyDown(SDL_Event* evt) {

}

void Ball::onKeyPressed(std::map<int, int>& keyMap) {

}


void Ball::onKeyUp(SDL_Event* evt) {
    switch (evt->key.keysym.sym)
    {
        case SDLK_SPACE :
            if(!isLaunched() && isPlaced())
            {
                setInitialSpeed();
                launch();
            }
            break;
        default:
            break;
    }
}


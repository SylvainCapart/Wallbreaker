/* 
 * File:   Ball.cpp
 * Author: Sylvain
 * 
 * Created on 6 janvier 2017, 18:46
 */

#include "Ball.h"

using namespace std;

Ball::Ball() {
    m_w = LITTLE_BALL_DIAMETER;
    m_h = LITTLE_BALL_DIAMETER;
    m_r = LITTLE_BALL_DIAMETER / 2;
    m_x = 0;
    m_y = 0;
    m_xCenter = 0;
    m_yCenter = 0;
    m_xSpeed = 0;
    m_ySpeed = 0;
    m_launched = false;
    m_placed = false;

}

Ball::~Ball() {
}

void Ball::setY_speed(float y_speed) {
  
    this->m_ySpeed = y_speed;

    // control of y speed component
    if (this->m_ySpeed > MAX_Y_BALL_SPEED)
        this->m_ySpeed = MAX_Y_BALL_SPEED;
    if (this->m_ySpeed < - MAX_Y_BALL_SPEED)
        this->m_ySpeed = - MAX_Y_BALL_SPEED;
    
 
}

void Ball::setX_speed(float x_speed) {
    
    this->m_xSpeed = x_speed;
    
    // control of x speed component
    if (this->m_xSpeed > MAX_X_BALL_SPEED)
        this->m_xSpeed = MAX_X_BALL_SPEED;
    if (this->m_xSpeed < - MAX_X_BALL_SPEED)
        this->m_xSpeed = - MAX_X_BALL_SPEED;
    
 
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
    
    m_x = m_x + m_xSpeed;
    m_y = m_y + m_ySpeed;
    m_xCenter = m_x + m_r;
    m_yCenter = m_y + m_r;
    
    
    if (m_xSpeed > MAX_X_BALL_SPEED)
        m_xSpeed = MAX_X_BALL_SPEED;
    if (m_ySpeed > MAX_Y_BALL_SPEED)
        m_ySpeed = MAX_Y_BALL_SPEED;
    
    if ((m_x + m_w < 0) || (m_x > WINDOW_WIDTH) || (m_y + m_h < 0) || (m_y > WINDOW_HEIGHT)) {
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

void Ball::setInitialSpeed(int racketId) {
    float initialSpeed = INITIAL_SPEED;
    
    switch (racketId) {
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
    }
}

/* Function : moveRight, moveLeft, moveDown, moveUp
 * Input : none
 * Output : none
 * Goal : for the ball to follow the racket when placed and not launched
 */

void Ball::moveRight() {
    m_x = m_x + RACKET_SPEED;
    m_xCenter = m_x + m_r;
    
}

void Ball::moveLeft() {
    m_x = m_x - RACKET_SPEED;
    m_xCenter = m_x + m_r;
    
}

void Ball::moveDown() {
    m_y = m_y + RACKET_SPEED;
    
    m_yCenter = m_y + m_r;
}

void Ball::moveUp() {
    m_y = m_y - RACKET_SPEED;
    
    m_yCenter = m_y + m_r;
}




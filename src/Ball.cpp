/* 
 * File:   Ball.cpp
 * Author: Sylvain
 * 
 * Created on 6 janvier 2017, 18:46
 */

#include "Ball.h"

using namespace std;

Ball::Ball() {
    w = LITTLE_BALL_DIAMETER;
    h = LITTLE_BALL_DIAMETER;
    r = LITTLE_BALL_DIAMETER / 2;
    
    x = 0;
    y = 0;
    xCenter = 0;
    yCenter = 0;
    x_speed = 0;
    y_speed = 0;
    launched = false;
    placed = false;

}

Ball::~Ball() {
}

void Ball::setY_speed(float y_speed) {
  
    this->y_speed = y_speed;
    
    
    // control of y speed component
    if (this->y_speed > MAX_Y_BALL_SPEED)
        this->y_speed = MAX_Y_BALL_SPEED;
    if (this->y_speed < - MAX_Y_BALL_SPEED)
        this->y_speed = - MAX_Y_BALL_SPEED;
    
 
}

void Ball::setX_speed(float x_speed) {
    
    this->x_speed = x_speed;
    
    // control of x speed component
    if (this->x_speed > MAX_X_BALL_SPEED)
        this->x_speed = MAX_X_BALL_SPEED;
    if (this->x_speed < - MAX_X_BALL_SPEED)
        this->x_speed = - MAX_X_BALL_SPEED;
    
 
}



/* Function : launch
 * Input : none
 * Output : none
 * Goal : triggers the launch of the ball while indicating it is not placed anymore on the racket
 */

void Ball::launch() {
    launched = true;
    placed = false;
}

/* Function : move
 * Input : none
 * Output : none
 * Goal : updates the position of the ball following its speed.
 *        checks speed limits
 *        checks position limits and reinitiate the ball if it goes past the borders
 */

void Ball::move() {
    
    x = x + x_speed;
    y = y + y_speed;
    xCenter = x + r;
    yCenter = y + r;
    
    
    if (x_speed > MAX_X_BALL_SPEED)
        x_speed = MAX_X_BALL_SPEED;
    if (y_speed > MAX_Y_BALL_SPEED)
        y_speed = MAX_Y_BALL_SPEED;
    
    if ((x + w < 0) || (x > WINDOW_WIDTH) || (y + h < 0) || (y > WINDOW_HEIGHT)) {
        launched = false;
        placed = false;
    }
    
    if (isnan(x) || isnan(y)) //to prevent some position errors and replace the ball if it occurs. 
        // The bug has been corrected and this case should not happen anymore.
    {
        launched = false;
        placed = false;
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
            x_speed = 0;
            y_speed = -initialSpeed;
            break;
            
        case TOP:
            x_speed = 0;
            y_speed = initialSpeed;
            break;
            
        case LEFT:
            x_speed = initialSpeed;
            y_speed = 0;
            break;
            
        case RIGHT:
            x_speed = -initialSpeed;
            y_speed = 0;
            break;
    }
}

/* Function : moveRight, moveLeft, moveDown, moveUp
 * Input : none
 * Output : none
 * Goal : for the ball to follow the racket when placed and not launched
 */

void Ball::moveRight() {
    x = x + RACKET_SPEED;
    xCenter = x + r;
    
}

void Ball::moveLeft() {
    x = x - RACKET_SPEED;
    xCenter = x + r;
    
}

void Ball::moveDown() {
    y = y + RACKET_SPEED;
    
    yCenter = y + r;
}

void Ball::moveUp() {
    y = y - RACKET_SPEED;
    
    yCenter = y + r;
}

/* Function : getBrickSegment
 * Input : const SDL_Rect& obstacle, the rectangle obstacle where we want to choose a rbound segment
 *         OBSTACLE_SEGMENT side, the side of the obstacle where the rebound will be done
 * Output : a SegmentPoints, i.e. the chosen rebound segment
 * Goal : select the right rebound segment on a rectangle obstacle
 */

SegmentPoints Ball::getBrickSegment(const SDL_Rect& obstacle, OBSTACLE_SEGMENT side) {
SegmentPoints AB;
    
    switch (side) {
        case OBSTACLE_BOTTOM:
            AB.A.x = obstacle.x;
            AB.A.y = obstacle.y + obstacle.h;
            AB.B.x = obstacle.x + obstacle.w;
            AB.B.y = obstacle.y + obstacle.h;
            break;
            
        case OBSTACLE_TOP:
            AB.A.x = obstacle.x;
            AB.A.y = obstacle.y;
            AB.B.x = obstacle.x + obstacle.w;
            AB.B.y = obstacle.y;
            break;
            
        case OBSTACLE_LEFT:
            AB.A.x = obstacle.x;
            AB.A.y = obstacle.y;
            AB.B.x = obstacle.x;
            AB.B.y = obstacle.y + obstacle.h;
            break;
            
        case OBSTACLE_RIGHT:
            AB.A.x = obstacle.x + obstacle.w;
            AB.A.y = obstacle.y;
            AB.B.x = obstacle.x + obstacle.w;
            AB.B.y = obstacle.y + obstacle.h;
            break;
    }
    return AB;
}

/* Function : rebound
 * 
 * Input : SDL_Rect obstacle, corresponding to a rectangle obstacle
 * 
 * Output : a SegmentPoints structure, what is two points and their x and y 
 * coordinates. This segment is the segment where the normal vector shall be
 * calculated. Following this normal vector, the ball will bounce with the right angle.
 * 
 * Goal : this function is called after a collision is detected between the ball
 * and an obstacle. The position of the ball is tested and the surface on which the
 * ball shall bounce following this position. There are 8 tested positions for 
 * the ball : LEFT, RIGHT, TOP, BOTTOM, TOPLEFT, BOTTOMLEFT, TOPRIGHT, BOTTOMRIGHT.
 * The center of the ball is not supposed to be inside the brick in any case. If 
 * the ball has a speed component that is equal to 0, the function shall ensure
 * that no parallel segment is returned.
 * Example : if the has a speed vector (0, 3) and hits at the top left of the brick,
 * the upper segment of the brick shall be returned, and not the left one.
 * 
 * TODO v0.2 : optimisation
 * This is handled also in the collision class for the middle racket. In game.cpp the Collision::rebound() should
 * replace the ball->rebound() function, which will be removed.
 */
    
SegmentPoints Ball::rebound(SDL_Rect obstacle) {
    SegmentPoints AB;
    
    if (xCenter <= obstacle.x && yCenter <= obstacle.y) 
        // ball is placed at the TOP LEFT of the brick
    {
        //different incidence angles
        if (x_speed > 0 && y_speed <= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
        else if (x_speed <= 0 && y_speed > 0)
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        else if (x_speed > 0 && y_speed > 0) //ball hitting the corner, coming from a top left origin
        {
            if (abs(x_speed) > abs(y_speed)) //selection of the rebound segment following the incidence angle
                AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
            else 
                AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        }
        // TODO v0.2 : the possibility and impact of the else case shall here be examined.
           
        
    } else if (xCenter >= (obstacle.x + obstacle.w) && yCenter <= obstacle.y) 
        // ball is placed at the TOP RIGHT of the brick
    {
        if (x_speed >= 0 && y_speed > 0)
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        else if (x_speed < 0 && y_speed <= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        else if (x_speed < 0 && y_speed > 0)
        {
            if (abs(x_speed) > abs(y_speed))
                AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
            else 
                AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        }
        // TODO v0.2 : the possibility and impact of the else case shall here be examined.
        
    } else if (xCenter <= (obstacle.x) && yCenter >= (obstacle.y + obstacle.h)) 
        // ball is placed at the BOTTOM LEFT of the brick
    {

        if (x_speed > 0 && y_speed >= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
        else if (x_speed <= 0 && y_speed < 0)
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        else if (x_speed > 0 && y_speed < 0)
        {
            if (abs(x_speed) > abs(y_speed))
                AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
            else 
                AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        }
        // TODO v0.2 : the possibility and impact of the else case shall here be examined.
        
    } else if (xCenter >= (obstacle.x + obstacle.w) && yCenter >= (obstacle.y + obstacle.h)) 
        // ball is placed at the BOTTOM RIGHT of the brick
    {
        if (x_speed >= 0 && y_speed < 0)
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        else if (x_speed < 0 && y_speed >= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        else if (x_speed < 0 && y_speed < 0)
        {
            if (abs(x_speed) <= abs(y_speed))
                AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
            else 
                AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        }
        // TODO v0.2 : the possibility and impact of the else case shall here be examined.
        
    } else if ((xCenter <= obstacle.x) && (obstacle.y < yCenter) && (yCenter < (obstacle.y + obstacle.h))) 
        // ball is placed at the LEFT of the brick
    {

        AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
        
    } else if ((obstacle.x < xCenter) && (xCenter < (obstacle.x + obstacle.w)) && yCenter <= obstacle.y) // ball is placed at the TOP of the brick
    {
        // ball is placed at the TOP of the brick
        AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        
    } else if (xCenter >= (obstacle.x + obstacle.w) && (obstacle.y < yCenter) && (yCenter < (obstacle.y + obstacle.h))) // ball is placed at the RIGHT of the brick
    {
        // ball is placed at the RIGHT of the brick
        AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        
    } else if ((obstacle.x < xCenter) && (xCenter < (obstacle.x + obstacle.w)) && (yCenter >= (obstacle.y + obstacle.h))) // ball is placed at the BOTTOM of the brick
    {
        // ball is placed at the BOTTOM of the brick
        AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        
    }
    else if ((obstacle.x < xCenter) && (xCenter < (obstacle.x + obstacle.w)) && (yCenter > obstacle.y) && (yCenter < (obstacle.y + obstacle.h))) // ball is INSIDE
    {
        //this case should not happen and is here for testing reasons.
        cout << "BALL INSIDE" << endl;
        
    }
    else //this case should not happen and is here for testing reasons.
    {
        cout << "NULL CASE" << endl;
        AB.A.x = 0;
        AB.A.y = 0;
        AB.B.x = 0;
        AB.B.y = 0;
    }
    
    return AB;
    
}


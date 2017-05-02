/* 
 * File:   Collision.cpp
 * Author: Sylvain
 * 
 * Except the rebound() function, this class is inspired by the two first chapters of the excellent guide
 * "Théorie des collisions" written by "Fred" on Openclassrooms.
 * Functions are mostly not commented but more details can be found on :
 * https://openclassrooms.com/courses/theorie-des-collisions/
 * 
 * Created on 15 janvier 2017, 22:29
 */

#include "Collision.h"
#include <vector>

using namespace std;

Collision::Collision() {
}

Collision::Collision(const Collision& orig) {
}

Collision::~Collision() {
}

/* Function : collisionCircleAABB
 * Input : Circle C1, SDL_Rect box1 : the circle corresponding to the ball and the
 * rectangle corresponding to the brick
 * Output : a boolean worth true if there is collision, false if not
 * Goal : detect a collision between the ball and a brick
 */

bool Collision::collisionCircleAABB(const Circle& C1, const SDL_Rect& box1) {
    SDL_Rect boxCercle = getBoxAroundCircle(C1);

    if (collisionAABBvsAABB(box1, boxCercle) == 0) {
        return false;
    }
    if (collisionPointCircle(box1.x, box1.y, C1) == 1

            || collisionPointCircle(box1.x, box1.y + box1.h, C1) == 1

            || collisionPointCircle(box1.x + box1.w, box1.y, C1) == 1

            || collisionPointCircle(box1.x + box1.w, box1.y + box1.h, C1) == 1) {
        return true;
    }
    if (collisionPointAABB(C1.x, C1.y, box1) == 1) {
        return true;
    }
    int projvertical = projectionOnSegment(C1.x, C1.y, box1.x, box1.y, box1.x, box1.y + box1.h);

    int projhorizontal = projectionOnSegment(C1.x, C1.y, box1.x, box1.y, box1.x + box1.w, box1.y);

    return (projvertical == 1 || projhorizontal == 1);
}

/* Function : collisionPointAABB
 * Input : int cursor_x, int cursor_y : position of a point
 *         const SDL_Rect& box : a rectangle
 * Output : a boolean worth true if the point is in the rectangle
 * Goal : detect the presence of a point ina  rectangle
 */

bool Collision::collisionPointAABB(int cursor_x, int cursor_y, const SDL_Rect& box) {

    return (cursor_x >= box.x
            && cursor_x < box.x + box.w
            && cursor_y >= box.y
            && cursor_y < box.y + box.h);
}

/* Function : projectionOnSegment
 *
 * Goal : determine the position of a point C regarding a segment AB.
 */

int Collision::projectionOnSegment(int Cx, int Cy, int Ax, int Ay, int Bx, int By) {

    int ACx = Cx - Ax;

    int ACy = Cy - Ay;

    int ABx = Bx - Ax;

    int ABy = By - Ay;

    int BCx = Cx - Bx;

    int BCy = Cy - By;

    int s1 = (ACx * ABx) + (ACy * ABy);

    int s2 = (BCx * ABx) + (BCy * ABy);

    if (s1 * s2 > 0)

        return 0;

    return 1;

}

/* Function : collisionAABBvsAABB
 * Input : const SDL_Rect& box1, const SDL_Rect& box2
 * Output : a boolean worth true if the 2 rectangles collide
 * Goal : detect the collision of two rectangles
 */

bool Collision::collisionAABBvsAABB(const SDL_Rect& box1, const SDL_Rect& box2) {

    return !((box2.x >= box1.x + box1.w)
            || (box2.x + box2.w <= box1.x)
            || (box2.y >= box1.y + box1.h)
            || (box2.y + box2.h <= box1.y));
}

/* Function : collisionPointCircle
 * Goal : detect the presence of a point inside a circle
 */

bool Collision::collisionPointCircle(int x, int y, const Circle& C) {

    int d2 = (x - C.x)*(x - C.x) + (y - C.y)*(y - C.y);

    if (d2 > C.radius * C.radius)

        return false;

    else

        return true;

}

/* Function : getBoxAroundCircle
 * Goal : get a SDL_rect rectangle surrounding the input circle
 */

SDL_Rect Collision::getBoxAroundCircle(const Circle& C1) {

    SDL_Rect circleRect;
    circleRect.x = C1.x - C1.radius;
    circleRect.y = C1.y - C1.radius;
    circleRect.w = C1.radius + C1.radius;
    circleRect.h = C1.radius + C1.radius;

    return circleRect;

}

/* Function : getNormal
 * Input : Point A, Point B, Point C : segment AB is the segment where we want to
 * get the normal vector, corresponding to the projection of point C on AB.
 * Output : a normalized vector on segment AB, on the point of collision
 * Goal : get a normalized vector at collision point to make the ball bounce on the surface
 * 
 */

Vector Collision::getNormal(const Point& A, const Point& B, const Point& C) {
    Vector AC, u, N;

    u.x = B.x - A.x;

    u.y = B.y - A.y;

    AC.x = C.x - A.x;

    AC.y = C.y - A.y;

    float parenthesis = u.x * AC.y - u.y * AC.x;

    N.x = -u.y * (parenthesis);

    N.y = u.x * (parenthesis);

    float norm = sqrt(N.x * N.x + N.y * N.y);

    N.x /= norm;

    N.y /= norm;

    return N;

}

/* Function : calculateVectorV2
 * Input : Vector v, Vector N : v is incoming vector corresponding to the ball
 * speed before collision, N is normal vector at impact point
 * Output : the new speed vector corresponding to the rebound of the ball with
 * the right angle regarding the normal vector
 * Goal : get the new speed vector with the right angle.
 */

Vector Collision::calculateVectorV2(const Vector& v, const Vector& N) {

    Vector v2;

    float pscal = (v.x * N.x + v.y * N.y);

    v2.x = v.x - 2 * pscal * N.x;

    v2.y = v.y - 2 * pscal * N.y;

    return v2;

}

/* Function : getBrickSegment
 * Input : const SDL_Rect& obstacle, the rectangle obstacle where we want to choose a rbound segment
 *         OBSTACLE_SEGMENT side, the side of the obstacle where the rebound will be done
 * Output : a SegmentPoints, i.e. the chosen rebound segment
 * Goal : select the right rebound segment on a rectangle obstacle
 */

SegmentPoints Collision::getBrickSegment(const SDL_Rect& obstacle, OBSTACLE_SEGMENT side) {
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
 * Input : const SDL_Rect & obstacle, reference to a rectangle obstacle
 *         const SDL_Rect & rectBall, reference to an other rectangle corresponding to the ball
 *         const Vector & ballSpeed, reference to the incidence speed vector of the ball
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
 * DONE v0.2 : optimisation
 * This is handled also in the ball class for the middle racket. In game.cpp the Collision::rebound() should
 * replace the ball->rebound() function, which will be removed. DONE
 */

SegmentPoints Collision::rebound(const SDL_Rect & obstacle, const SDL_Rect & rectBall, const Vector & ballSpeed) {
    SegmentPoints AB;
    Point rectBallCenter(rectBall.x + rectBall.w / 2, rectBall.y + rectBall.h / 2);

    if (rectBallCenter.x <= obstacle.x && rectBallCenter.y <= obstacle.y) 
        // ball is placed at the TOP LEFT of the brick
    {
        if (ballSpeed.x > 0 && ballSpeed.y <= 0) // selection following incident speed
            AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
        else if (ballSpeed.x <= 0 && ballSpeed.y > 0)
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        else if (ballSpeed.x > 0 && ballSpeed.y > 0) {
            if (abs(ballSpeed.x) > abs(ballSpeed.y))
                AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
            else
                AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        } else {
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        }


    } else if (rectBallCenter.x >= (obstacle.x + obstacle.w) && rectBallCenter.y <= obstacle.y) 
        // ball is placed at the TOP RIGHT of the brick
    {
        if (ballSpeed.x >= 0 && ballSpeed.y > 0)
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        else if (ballSpeed.x < 0 && ballSpeed.y <= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        else if (ballSpeed.x < 0 && ballSpeed.y > 0) {
            if (abs(ballSpeed.x) > abs(ballSpeed.y))
                AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
            else
                AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        } else {
            AB = getBrickSegment(obstacle, OBSTACLE_TOP);
        }

    } else if (rectBallCenter.x <= (obstacle.x) && rectBallCenter.y >= (obstacle.y + obstacle.h)) 
        // ball is placed at the BOTTOM LEFT of the brick
    {
        if (ballSpeed.x > 0 && ballSpeed.y >= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
        else if (ballSpeed.x <= 0 && ballSpeed.y < 0)
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        else if (ballSpeed.x > 0 && ballSpeed.y < 0) {
            if (abs(ballSpeed.x) > abs(ballSpeed.y))
                AB = getBrickSegment(obstacle, OBSTACLE_LEFT);
            else
                AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        } else {
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        }

    } else if (rectBallCenter.x >= (obstacle.x + obstacle.w) && rectBallCenter.y >= (obstacle.y + obstacle.h)) // ball is placed at the BOTTOM RIGHT of the brick
    {
        if (ballSpeed.x >= 0 && ballSpeed.y < 0)
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        else if (ballSpeed.x < 0 && ballSpeed.y >= 0)
            AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        else if (ballSpeed.x < 0 && ballSpeed.y < 0) {
            if (abs(ballSpeed.x) <= abs(ballSpeed.y))
                AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
            else
                AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);
        } else {
            AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);
        }

    } else if ((rectBallCenter.x <= obstacle.x) && (obstacle.y < rectBallCenter.y) && (rectBallCenter.y < (obstacle.y + obstacle.h))) 
        // ball is placed at the LEFT of the brick
    {
        AB = getBrickSegment(obstacle, OBSTACLE_LEFT);

    } else if ((obstacle.x < rectBallCenter.x) && (rectBallCenter.x < (obstacle.x + obstacle.w)) && rectBallCenter.y <= obstacle.y) 
        // ball is placed at the TOP of the brick
    {
        AB = getBrickSegment(obstacle, OBSTACLE_TOP);

    } else if (rectBallCenter.x >= (obstacle.x + obstacle.w) && (obstacle.y < rectBallCenter.y) && (rectBallCenter.y < (obstacle.y + obstacle.h))) 
        // ball is placed at the RIGHT of the brick
    {
        AB = getBrickSegment(obstacle, OBSTACLE_RIGHT);

    } else if ((obstacle.x < rectBallCenter.x) && (rectBallCenter.x < (obstacle.x + obstacle.w)) && (rectBallCenter.y >= (obstacle.y + obstacle.h))) 
        // ball is placed at the BOTTOM of the brick
    {
        AB = getBrickSegment(obstacle, OBSTACLE_BOTTOM);

    }
    else if ((obstacle.x <= rectBallCenter.x) && (rectBallCenter.x <= (obstacle.x + obstacle.w)) && (rectBallCenter.y >= obstacle.y) && (rectBallCenter.y <= (obstacle.y + obstacle.h))) // ball is INSIDE
    {
        //this case (BALL INSIDE) should not happen and is here for testing reasons.
        
        AB.A.x = 0;
        AB.A.y = 0;
        AB.B.x = 0;
        AB.B.y = 0;

    } else {
        //this case (ELSE CASE) should not happen and is here for testing reasons.

        AB.A.x = 0;
        AB.A.y = 0;
        AB.B.x = 0;
        AB.B.y = 0;
    }

    return AB;

}

/* Function : projectionI
 * Goal : return I, the projection of point C on segment AB
 */

Point Collision::projectionI(const Point& A, const Point& B, const Point& C) {

    Vector u, AC;

    u.x = B.x - A.x;

    u.y = B.y - A.y;

    AC.x = C.x - A.x;

    AC.y = C.y - A.y;

    float ti = (u.x * AC.x + u.y * AC.y) / (u.x * u.x + u.y * u.y);

    Point I;

    I.x = A.x + ti * u.x;

    I.y = A.y + ti * u.y;

    return I;

}

float Collision::getDistanceBetweenTwoPoints(const Point& A, const Point& B) {
    float a = abs(A.x - B.x);
    float b = abs(A.y - B.y);

    return sqrt((a * a) + (b * b));
}




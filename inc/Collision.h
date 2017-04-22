/* 
 * File:   Collision.h
 * Author: Sylvain
 *
 * Created on 15 janvier 2017, 22:29
 */

#ifndef COLLISION_H
#define	COLLISION_H

#include "Ball.h"
#include <SDL.h>
#include "Const.h"
#include "Types.h"

/* Except the rebound() function, this class is inspired by the two first chapters of the excellent guide
 * "Théorie des collisions" written by "Fred" on Openclassrooms.
 * Functions are mostly not commented but more details can be found on :
 * https://openclassrooms.com/courses/theorie-des-collisions/
 */

class Collision {
public:
    // See Collision.cpp for functionnal description
    Collision();
    Collision(const Collision& orig);
    virtual ~Collision();
    
    bool static collisionCircleAABB(const Circle& C1, const SDL_Rect& box1);
    int static projectionOnSegment(int Cx,int Cy,int Ax,int Ay,int Bx,int By);
    bool static collisionAABBvsAABB(const SDL_Rect& box1, const SDL_Rect& box2);
    bool static collisionPointCircle(int x,int y,const Circle& C);
    bool static collisionPointAABB(int curseur_x,int curseur_y,const SDL_Rect& box);
    SDL_Rect static getBoxAroundCircle(const Circle& C1);
    Point static projectionI(const Point& A, const Point& B, const Point& C);
    Vector static getNormal(const Point& A, const Point&  B, const Point& C);
    Vector static calculateVectorV2(const Vector& v,const Vector& N);

    SegmentPoints static rebound(const SDL_Rect & obstacle, const SDL_Rect & rectBall, const Vector & ballSpeed);
    SegmentPoints static getBrickSegment(const SDL_Rect& obstacle, OBSTACLE_SEGMENT side);
    float static getDistanceBetweenTwoPoints( const Point& A, const Point& B);


private:
    
};

#endif	/* COLLISION_H */


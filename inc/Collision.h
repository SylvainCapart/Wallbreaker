/* 
 * File:   Collision.h
 * Author: Sylvain
 *
 * Created on 15 janvier 2017, 22:29
 */

#ifndef COLLISION_H
#define COLLISION_H

#include "Types.h"
#include <vector>
#include <SDL.h>

/* Except the rebound() function, this class is inspired by the two first chapters of the excellent guide
 * "Théorie des collisions" written by "Fred" on Openclassrooms.
 * More details can be found on :
 * https://openclassrooms.com/courses/theorie-des-collisions/
 */

class Collision {
public:
    // See Collision.cpp for functionnal description
    Collision();
    Collision(const Collision& orig);
    virtual ~Collision();

    /// \param C1 : the circle corresponding to the ball
    /// \param box1 : the rectangle corresponding to the obstacle
    /// \return : a boolean worth true if there is collision, false if not
    /// \goal : detect a collision between the ball and an obstacle
    bool static collisionCircleAABB(const Circle& C1, const SDL_Rect& box1);

    /// \goal : determine the position of a point C regarding a segment AB.
    int static projectionOnSegment(int Cx, int Cy, int Ax, int Ay, int Bx, int By);

    /// \param box1 : a ractangle
    /// \param box2 : another rectangle
    /// \return a boolean worth true if the 2 rectangles collide
    /// \goal : detect the collision of two rectangles
    bool static collisionAABBvsAABB(const SDL_Rect& box1, const SDL_Rect& box2);

    /// \goal : detect the presence of a point inside a circle
    bool static collisionPointCircle(int x, int y, const Circle& C);

    /// \param curseur_x : x position of a point
    /// \param curseur_y : y position of a point
    /// \param box : a rectangle
    /// \return : a boolean worth true if the point is in the rectangle
    /// \goal : detect the presence of a point in a rectangle
    bool static collisionPointAABB(int curseur_x, int curseur_y, const SDL_Rect& box);

    /// \goal : get a SDL_rect rectangle surrounding the input circle
    SDL_Rect static getBoxAroundCircle(const Circle& C1);

    /// \goal : return I, the projection of point C on segment AB
    Point static projectionI(const Point& A, const Point& B, const Point& C);

    /// \param A
    /// \param B
    /// \param C : the point which is projected on segment AB
    /// \return a normalized vector on segment AB, on the point of collision
    /// \goal : get a normalized vector at collision point to make the ball bounce on the surface
    Vector static getNormal(const Point& A, const Point& B, const Point& C);

    /// \param v : v is incoming vector corresponding to the ball speed before collision
    /// \param N : N is normal vector at impact point
    /// \return : the new speed vector corresponding to the rebound of the ball with 
    /// the right angle regarding the normal vector
    /// \goal : get the new speed vector with the right angle.
    Vector static calculateVectorV2(const Vector& v, const Vector& N);

    /// \param obstacle : reference to a rectangle obstacle
    /// \param rectBall : reference to an other rectangle corresponding to the ball
    /// \param ballSpeed : reference to the incidence speed vector of the ball
    /// \return : a SegmentPoints structure, what is two points and their x and y 
    /// coordinates. This segment is the segment where the normal vector shall be
    /// calculated. Following this normal vector, the ball will bounce with the right angle.
    ///
    /// \goal : Goal : this function is called after a collision is detected between the ball
    /// and an obstacle. The position of the ball is tested and the surface on which the
    /// ball shall bounce following this position. There are 8 tested positions for 
    /// the ball : LEFT, RIGHT, TOP, BOTTOM, TOPLEFT, BOTTOMLEFT, TOPRIGHT, BOTTOMRIGHT.
    /// The center of the ball is not supposed to be inside the brick in any case. If 
    /// the ball has a speed component that is equal to 0, the function shall ensure
    /// that no parallel segment is returned.
    /// Example : if the has a speed vector (0, 3) and hits at the top left of the brick,
    /// the upper segment of the brick shall be returned, and not the left one.
    SegmentPoints static rebound(const SDL_Rect & obstacle, const SDL_Rect & rectBall, const Vector & ballSpeed);

    /// \param obstacle : the rectangle obstacle where we want to choose a rbound segment
    /// \param side : the side of the obstacle where the rebound will be done
    /// \return : a SegmentPoints, i.e. the chosen rebound segment
    /// \goal : select the right rebound segment on a rectangle obstacle
    SegmentPoints static getBrickSegment(const SDL_Rect& obstacle, OBSTACLE_SEGMENT side);
    
    /// 
    /// \param A
    /// \param B
    /// \return : a float worth the distance between A and B
    float static getDistanceBetweenTwoPoints(const Point& A, const Point& B);


private:

};

#endif /* COLLISION_H */


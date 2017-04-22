/* 
 * File:   Types.h
 * Author: Sylvain
 * 
 * Types of the projects.
 *
 * Created on 5 février 2017, 11:01
 */

#ifndef TYPES_H
#define TYPES_H

#include <ostream>
#include <SDL.h>

class Color { // This class could have its own .h and .cpp
public:
    int r, g, b;

    Color(int r, int g, int b) : r(r), g(g), b(b) {
    }
};

struct Circle {
    int x, y; //center coordinates
    int radius;
};

//Trace operator overriding
inline std::ostream& operator<<(std::ostream& os, const Circle& c) {
     os << "struct Circle {x=" << c.x << ", y=" << c.y << ", radius=" << c.radius << "}";
     return os;
}

inline std::ostream& operator<<(std::ostream& os, const SDL_Rect& r) {
     os << "struct SDL_Rect {h=" << r.h << ", w=" << r.w << ", x=" << r.x << ", y=" << r.y << "}";
     return os;
}



struct Point {
    float x, y;

    Point() : x(0), y(0) {
    };
    Point(float X, float Y) : x(X), y(Y) { 
    };
    Point(const Point & anotherPoint) {
        this->x = anotherPoint.x;
        this->y = anotherPoint.y;
    }
};



struct SegmentPoints {
    Point A;
    Point B;
    SegmentPoints() : A(0,0), B(0,0){
    };
    
    SegmentPoints(const Point& A, const Point& B)
    {
        this->A.x = A.x;
        this->A.y = A.y;
        this->B.x = B.x;
        this->B.y = B.y;
    }
};

inline std::ostream& operator<<(std::ostream& os, const SegmentPoints& AB) {
     os << "struct SegmentPoints : A(" << AB.A.x << ", " << AB.A.y << ") / B(" << AB.B.x << ", " << AB.B.y << ")";
     return os;
}

inline std::ostream& operator<<(std::ostream& os, const Point& A) {
     os << "Point(" << A.x << ", " << A.y << ") ";
     return os;
}

struct Vector {
    float x, y;
    Vector(){};
    Vector(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};



enum {
    DISPLAY_WIDTH = WINDOW_WIDTH
    , DISPLAY_HEIGHT = WINDOW_HEIGHT
    , UPDATE_INTERVAL = 1000 / 60
    , RACKET_SPEED = 1
};



enum ORIENTATION { // orientation of the rackets
    HORIZONTAL,
    VERTICAL
};

enum GAMESTATE { // state of the game
    MENU,
    GAME,
    WON
};

enum AXIS {
    XAXIS,
    YAXIS
};

enum POSITION { // position of a racket
    BOTTOM,
    TOP,
    LEFT,
    RIGHT,
    MIDDLE
};

enum OBSTACLE_SEGMENT { // position of a segment on a SDL_rect obstacle
    OBSTACLE_BOTTOM,
    OBSTACLE_TOP,
    OBSTACLE_LEFT,
    OBSTACLE_RIGHT
    
};

enum BUTTON_STATE {
    NO_ACTION,
    MOUSE_OVER,
    PRESSED
};

#endif /* TYPES_H */


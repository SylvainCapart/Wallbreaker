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
#include "Const.h"

class Color { // This class could have its own .h and .cpp
public:
    Uint8 r, g, b;
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
    Vector (const Vector &vect)
    {
        this->x = vect.x;
        this->y = vect.y;
    }
    Vector& operator =(Vector const& vect) {
        this->x = vect.x;
        this->y = vect.y;
        return *this;
    }
};


enum T_ORIENTATION { // orientation of the rackets
    HORIZONTAL,
    VERTICAL
};


enum AXIS {
    XAXIS,
    YAXIS
};

enum T_RACKET_POSITION { // position of a racket
    BOTTOM = 0,
    TOP = 1,
    LEFT = 2,
    RIGHT = 3,
    MIDDLE = 4,
    POSITION_NB_ = (MIDDLE +1)
};

enum T_KEY_BIND {
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_Z,
    K_S,
    K_Q,
    K_D,
    K_SPACE,
    K_NULL
};

enum OBSTACLE_SEGMENT { // position of a segment on a SDL_rect obstacle
    OBSTACLE_BOTTOM,
    OBSTACLE_TOP,
    OBSTACLE_LEFT,
    OBSTACLE_RIGHT
    
};

enum BUTTON_STATE { // state of the button following mouse actions
    NO_ACTION,
    MOUSE_OVER,
    PRESSED
};

enum STATE { // states possible for a IState
    MENU_STATE,
    GAME_STATE,
    EDIT_STATE
};

enum T_MENU_STATE { // states possible for the main menu
    MAIN_MENU,
    LEVEL_CHOICE_MENU,
    SCORE_MENU
};

enum MOUSE_LISTENER_ID {
    B_MENU_START,
    B_MENU_QUIT,
    B_RANDOM_LEVEL,
    B_DEMO_LEVEL,
    B_EDIT_LEVEL,
    B_BACK_FROM_LEVEL_CHOICE,
    B_BACK_FROM_EDIT,
    B_PLAY_EDITED_LEVEL,
    B_CLEAR_EDITED_LEVEL
};

enum DRAWABLE_TYPE {
    BUTTON_TYPE,
    TEXT_TYPE
};

enum T_LOAD_LEVEL {
    RANDOM_LEVEL,
    DEMO_LEVEL,
    EDIT_LEVEL
};

enum T_GAME_MODE {
    PLAY_MODE,
    EDIT_MODE
};

enum T_FONT {
    ARROW_FONT,
    INFO_FONT
};

#endif /* TYPES_H */


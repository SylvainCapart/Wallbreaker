/* 
 * File:   Brick.h
 * Author: Sylvain
 *
 * This class is not used yet and is here for further feature implementations.
 * For the moment the brick level is a series of characters : '0' for empty spaces,
 * '1' for 1-hit bricks and '2' for 2-hits bricks. This series fills a 2 dimensions
 * table (called brickMap) containing corresponding int.
 * 
 * TODO : improvement
 * Replace the int table by a Brick table. That seems to be a better solution for 
 * further features like effects linked to the bricks : area effects, bonus dropping, etc ...
 *
 * Created on 29 janvier 2017, 10:22
 */

#ifndef BRICK_H
#define	BRICK_H

#include <SDL.h>
#include "Const.h"

class Brick {
public:

    Brick(int x, int y, int resistance);

    void SetResistance(int resistance) {this->resistance = resistance;}
    int GetResistance() const {return resistance;}
    
    Brick& operator --() {
        --this->resistance;
        return *this;
    }
    
    const SDL_Rect& getRect() const {
        return m_renderArea;
    }
    
    /* remove all */
    void SetH(int h) {this->h = h;}
    int GetH() const {return h;}
    
    void SetW(int w) {this->w = w;}
    int GetW() const {return w;}
    
    void SetY(int y) {this->y = y;}
    int GetY() const {return y;}
    
    void SetX(int x) {this->x = x;}
    int GetX() const {return x;}
    
private:
    // to remove
    int x, y, w, h; // x and y -> Rectangle origin (not the center of the brick)
    SDL_Rect m_renderArea;
    int resistance; // number of times the brick shall be hit before disappearing
    
};

#endif	/* BRICK_H */


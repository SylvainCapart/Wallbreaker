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


#include "IDrawable.h"

class Brick : public IDrawable{
public:

    virtual ~Brick();

    /// \goal : true if (x,y) belongs to the IDrawable rectangle
    bool detectSelection(int x, int y) const;

    /// \goal : draw the IDrawable
    void draw() const;

    /// \param x : x coordinate of the origin of the SDL_Rect corresponding to the brick
    /// \param y : y coordinate of the origin of the SDL_Rect corresponding to the brick
    /// \param resistance : number of times the brick shall be hit before disappearing
    Brick(int x, int y, int resistance, SDL_Renderer * renderer);

    /// \goal : getter and setter for m_resistance
    void SetResistance(int resistance) {this->m_resistance = resistance;}
    int GetResistance() const {return m_resistance;}
    
    /// \goal : overriding -- to decrease the brick's resistance
    Brick& operator --() {
        if (this->m_resistance >= 0 && this->m_resistance < 4)
        --this->m_resistance;
        return *this;
    }

    void fillRectGame(SDL_Rect* rc, const Color& color) const;
    
    void SetH(int h) {this->m_h = h;}
    int GetH() const {return m_h;}
    
    void SetW(int w) {this->m_w = w;}
    int GetW() const {return m_w;}
    
    void SetY(int y) {this->m_y = y;}
    int GetY() const {return m_y;}
    
    void SetX(int x) {this->m_x = x;}
    int GetX() const {return m_x;}
    
private:
    int m_x, m_y, m_w, m_h; // m_x and m_y -> Rectangle origin (not the center of the brick)
    int m_resistance; // number of times the brick shall be hit before disappearing
    SDL_Rect m_brickRect; //rectangle corresponding to the brick
    SDL_Renderer * m_renderer;
};

#endif	/* BRICK_H */


/* 
 * File:   Brick.cpp
 * Author: Sylvain
 * 
 * Created on 29 janvier 2017, 10:22
 */

#include "Brick.h"
#include "Const.h"

Brick::Brick(int x, int y, int resistance, SDL_Renderer * renderer):m_renderer(renderer) {
    m_x = x;
    m_y = y;
    m_w = BRICK_WIDTH;
    m_h = BRICK_HEIGHT;
    m_brickRect.x = m_x;
    m_brickRect.y = m_y;
    m_brickRect.w = m_w;
    m_brickRect.h = m_h;
    m_resistance = resistance;

}

Brick::~Brick() {

}


void Brick::fillRectGame(SDL_Rect* rc, const Color& color) const {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, rc);
}

bool Brick::detectSelection(int x, int y) const {
    return (x > m_x && x < m_x + m_w && y > m_y && y < m_y + m_h );
}

void Brick::draw() const { 
    SDL_Rect brickRect = {m_x, m_y, m_w, m_h};
    switch(m_resistance)
    {
        case 0:
            return;
        case 1:
            fillRectGame(&brickRect, Colors::st_yellow); // sets the color + draw the rectangle with it
            break;
        case 2:
            fillRectGame(&brickRect, Colors::st_paleOrange);
            break;
        case 3:
            fillRectGame(&brickRect, Colors::st_paleRed);
            break;
        case 4:
            fillRectGame(&brickRect, Colors::st_grey);
            break;
    }

    SDL_SetRenderDrawColor(m_renderer, Colors::st_black.r, Colors::st_black.g, Colors::st_black.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_renderer, m_x, m_y, m_x + m_w, m_y);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y, m_x + m_w, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y + m_h, m_x, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x, m_y + m_h, m_x, m_y);
    
}






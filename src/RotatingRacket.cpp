/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RotatingRacket.cpp
 * Author: user
 * 
 * Created on 6 juillet 2017, 18:02
 */

#include "RotatingRacket.h"

using namespace std;

RotatingRacket::RotatingRacket(SDL_Renderer * renderer, int x, int y, const Color& fillColor, 
    const Color& borderColor, T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition):
Racket(renderer, x, y, HORIZONTAL, fillColor, borderColor, firstBind, secondBind, racketPosition), m_middleRacketAngle(0)  {
    
m_racketTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RACKET_WIDTH, RACKET_HEIGHT);
m_middleRacketRotationActivated = true;   
}

RotatingRacket::~RotatingRacket() {
    SDL_DestroyTexture(m_racketTexture);
}


void RotatingRacket::draw() const {
    SDL_Rect racketRect = {m_x, m_y, m_w, m_h};
    
    SDL_SetRenderTarget(m_renderer, m_racketTexture);
    SDL_SetRenderDrawColor(m_renderer, Colors::st_blueGreen.r, Colors::st_blueGreen.g, Colors::st_blueGreen.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, NULL);
    SDL_SetRenderDrawColor(m_renderer, m_borderColor.r,m_borderColor.g,m_borderColor.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_renderer, m_x, m_y, m_x + m_w, m_y);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y, m_x + m_w, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x + m_w, m_y + m_h, m_x, m_y + m_h);
    SDL_RenderDrawLine(m_renderer, m_x, m_y + m_h, m_x, m_y);
    SDL_SetRenderTarget(m_renderer, NULL);
    
    SDL_QueryTexture(m_racketTexture, NULL, NULL, &racketRect.w, &racketRect.h);
    
    //render a rectangle rotated by middleRacketAngle
    SDL_RenderCopyEx(m_renderer, m_racketTexture, NULL, &racketRect, m_middleRacketAngle, NULL, SDL_FLIP_NONE);
}

void RotatingRacket::onKeyDown(SDL_Event* evt) {

}

void RotatingRacket::onKeyUp(SDL_Event* evt) {

}

void RotatingRacket::onKeyPressed(std::map<int, int>& keyMap) {
    if (keyMap[SDLK_SPACE] && (K_SPACE == m_firstBind || K_SPACE == m_secondBind)) 
        rotate();
}

void RotatingRacket::rotate() {
    if (m_middleRacketRotationActivated) 
        m_middleRacketAngle = (m_middleRacketAngle + MID_RACKET_ANGLE_SPEED) % ANGLE_MODULO;
}

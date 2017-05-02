/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Text.cpp
 * Author: user
 * 
 * Created on 1 mai 2017, 11:58
 */

#include <SDL2/SDL_render.h>
#include <cassert>
#include "Text.h"

using namespace std;

Text::Text(SDL_Renderer * renderer, TTF_Font * font, const string & text, const SDL_Color & color, int x, int y, int w, int h)
:m_text(text), m_color(color), m_font(font), m_renderer(renderer)
{
    m_textRect.x = x;
    m_textRect.y = y;
    m_textRect.w = w;
    m_textRect.h = h;
    
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(m_font, m_text.c_str(), m_color);
    m_textTexture = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);
    
    SDL_FreeSurface(surfaceMessage);
}


Text::~Text() {
    SDL_DestroyTexture(m_textTexture);
}

void Text::draw() const {
    SDL_RenderCopy(m_renderer, m_textTexture, NULL, &m_textRect);
}

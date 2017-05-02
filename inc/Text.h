/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Text.h
 * Author: user
 *
 * Created on 1 mai 2017, 11:58
 */

#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL.h>
#include "Const.h"
#include "Types.h"
#include <SDL_ttf.h>
#include <iostream>
#include "IDrawable.h"

class Text : public IDrawable {
public:
    
    Text(SDL_Renderer * renderer, TTF_Font * font, const std::string & text, const SDL_Color & color, int x, int y, int w, int h);
    virtual ~Text();
    void draw() const;

    /// \param x
    /// \param y
    /// \return false
    /// \comment : not used here
    virtual bool detectSelection(int x, int y) const {
        return false;
    }

    /// \return drawable id (int)
    /// \comment : not used here
    virtual const int getDrawableId() const {
        return 1;
    }

    virtual void setButtonState(BUTTON_STATE buttonState) const {
    }

private:
    
    const std::string m_text; // text to write
    const SDL_Color m_color; // color of the rext
    TTF_Font * m_font; // font of the text
    SDL_Texture * m_textTexture; //texture needed to create graphics
    SDL_Rect m_textRect; // area where we draw the text
    SDL_Renderer * m_renderer; // renderer where we draw graphics
};

#endif /* TEXT_H */


/* 
 * File:   Button.cpp
 * Author: Sylvain
 * 
 * Created on 8 avril 2017, 12:16
 */

#include "Button.h"

Button::Button(const SDL_Rect & buttonRect, SDL_Renderer * renderer, const string & text, const SDL_Color
& textColor , TTF_Font & textFont, SDL_Texture * buttonImage[3])
: buttonRect(buttonRect) {
    
    this->buttonImage[NO_ACTION] = buttonImage[NO_ACTION];
    this->buttonImage[MOUSE_OVER] = buttonImage[MOUSE_OVER];
    this->buttonImage[PRESSED] = buttonImage[PRESSED];
    
    this->textOrigin = BUTTON_TEXT_ORIGIN;
    this->textMargin = BUTTON_TEXT_MARGIN;
    
    this->textRect.x = buttonRect.x + textOrigin;
    this->textRect.y = buttonRect.y + textOrigin;
    this->textRect.w = buttonRect.w - textMargin;
    this->textRect.h = buttonRect.h - textMargin;
    
    
    this->buttonState = NO_ACTION;
    this->text = text;
    
    this->textColor = textColor;
    
    
    SDL_Surface * buttonTextSurface = TTF_RenderText_Blended(&textFont, text.c_str(), textColor);
    buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
    
    SDL_FreeSurface(buttonTextSurface);
    
}



Button::~Button() {
    
}


bool Button::detectSelection(int x, int y) const {
    
    return (x > buttonRect.x && x < buttonRect.x + buttonRect.w && y > buttonRect.y && y < buttonRect.y + buttonRect.h );
    
}

void Button::draw(SDL_Renderer * renderer) const {
    
    SDL_RenderCopy(renderer, buttonImage[buttonState], NULL, &buttonRect);
    SDL_RenderCopy(renderer, buttonTextTexture, NULL, &textRect);
    
}



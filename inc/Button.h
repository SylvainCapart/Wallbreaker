/* 
 * File:   Button.h
 * Author: Sylvain
 *
 * Created on 8 avril 2017, 12:16
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <string>
#include <SDL.h>
#include "Const.h"
#include "Types.h"
#include <SDL_ttf.h>


using namespace std;

class Button {
public:
    Button(const SDL_Rect & buttonRect, SDL_Renderer * renderer, const string & text, 
            const SDL_Color & textColor ,  TTF_Font & textFont, SDL_Texture * buttonImage[3]);

    virtual ~Button();

    bool detectSelection(int x, int y) const; // true if (x,y) inside the buttonRect
    
    void draw(SDL_Renderer * renderer) const;

    void setButtonState(BUTTON_STATE buttonState) {
        this->buttonState = buttonState;
    }

    BUTTON_STATE getButtonState() const {
        return buttonState;
    }

    void setText(const std::string & text) {
        this->text = text;
    }

    const std::string & getText() const {
        return text;
    }

    SDL_Rect * getButtonRect() {
        return & buttonRect;
    }

    SDL_Rect * getTextRect() {
        return & textRect;
    }
    
    

private:
    SDL_Rect buttonRect, textRect;

    SDL_Color textColor;

    SDL_Texture * buttonImage[3];
    SDL_Texture * buttonTextTexture;
    BUTTON_STATE buttonState;
    std::string text;
    int textMargin, textOrigin;
};

#endif	/* BUTTON_H */


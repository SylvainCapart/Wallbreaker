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
#include <iostream>
#include "IDrawable.h"
#include "Text.h"



class Button : public IDrawable {
public:
    
    /// \param renderer : renderer where the button will be drawn
    /// \param buttonText : object Text, containing the text we want to display on the button (also a IDrawable)
    /// \param buttonId : ID of the button
    /// \param buttonRect : rectangle corresponding to the drawing area of the button
    /// \param buttonImage : vector of textures (hard value to replace), containing the different images of the button
    Button(SDL_Renderer * renderer, Text * buttonText, const BUTTON_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[3]);
    
    virtual ~Button();
    
    /// \param x
    /// \param y 
    /// \return : true if (x,y) is in the button rectangle
    bool detectSelection(int x, int y) const; 
    
    /// \goal : drawing of the IDrawable
    void draw() const;
    
    void setButtonState(BUTTON_STATE buttonState) {
        this->m_buttonState = buttonState;
    }
    BUTTON_STATE getButtonState() const {
        return m_buttonState;
    }
    SDL_Rect * getButtonRect() {
        return & m_buttonRect;
    }
    const int getDrawableId() const {
        return m_buttonId;
    }

private:
    SDL_Rect m_buttonRect;
    int m_buttonId;
    SDL_Texture * m_buttonImage[3];
    BUTTON_STATE m_buttonState;
    SDL_Renderer * m_renderer;
    Text * m_buttonText;
    

};

#endif	/* BUTTON_H */


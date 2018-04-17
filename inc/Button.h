/* 
 * File:   Button.h
 * Author: Sylvain
 *
 * Created on 8 avril 2017, 12:16
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "IMouseListener.h"
#include "IDrawable.h"
#include "Text.h"
#include <SDL_ttf.h>

class Button : public IDrawable, public IMouseListener {
public:
    
    /// \param renderer : renderer where the button will be drawn
    /// \param buttonText : object Text, containing the text we want to display on the button (also a IDrawable)
    /// \param buttonId : ID of the button
    /// \param buttonRect : rectangle corresponding to the drawing area of the button
    /// \param buttonImage : vector of textures (hard value to replace), containing the different images of the button
    Button(SDL_Renderer * renderer, Text * buttonText, const MOUSE_LISTENER_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[BUTTON_STATES_NUMBER]);
    
    /// \param renderer : renderer where the button will be drawn
    /// \param buttonId : ID of the button
    /// \param buttonRect : rectangle corresponding to the drawing area of the button
    /// \param buttonImage : vector of textures (hard value to replace), containing the different images of the button
    Button(SDL_Renderer * renderer, const MOUSE_LISTENER_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[BUTTON_STATES_NUMBER]);
    
    virtual ~Button();
    
    /// \param x
    /// \param y 
    /// \return : true if (x,y) is in the button rectangle
    bool detectSelection(int x, int y) const; 
    
    /// \goal : drawing of the IDrawable
    void draw() const;
    
    // listening to mouse action via IMouseListener
    void onClickDown(SDL_Event * evt);
    void onClickUp(SDL_Event * evt);
    void onMouseMotion(SDL_Event* evt);
    void noFocus();

    
    void setButtonState(BUTTON_STATE buttonState) {
        this->m_buttonState = buttonState;
    }
    BUTTON_STATE getButtonState() const {
        return m_buttonState;
    }
    SDL_Rect * getButtonRect() {
        return & m_buttonRect;
    }
    int getMouseListenerId() const {
        return m_buttonId;
    }
  

    void setText(std::string & buttonText, TTF_Font * font);
    
private:
    SDL_Rect m_buttonRect;
    int m_buttonId;
    SDL_Texture * m_buttonImage[BUTTON_STATES_NUMBER];
    BUTTON_STATE m_buttonState;
    SDL_Renderer * m_renderer;
    Text * m_buttonText;
    bool m_textInstanceExists;


};

#endif	/* BUTTON_H */


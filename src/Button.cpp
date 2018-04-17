/* 
 * File:   Button.cpp
 * Author: Sylvain
 * 
 * Created on 8 avril 2017, 12:16
 */

#include "Button.h"
#include "Fonts.h"
#include "Const.h"
#include "Types.h"
#include <SDL.h>
#include <iostream>
#include <string>


using namespace std;

Button::Button(SDL_Renderer * renderer, Text * buttonText, const MOUSE_LISTENER_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[BUTTON_STATES_NUMBER])
: m_buttonRect(buttonRect),m_buttonId(buttonId), m_renderer(renderer)
{   
    m_buttonImage[NO_ACTION] = buttonImage[NO_ACTION];
    m_buttonImage[MOUSE_OVER] = buttonImage[MOUSE_OVER];
    m_buttonImage[PRESSED] = buttonImage[PRESSED];       
    m_buttonState = NO_ACTION;
    m_buttonText = buttonText;
    m_buttonText->setTextRect(m_buttonRect.x + m_buttonRect.w/40, m_buttonRect.y + m_buttonRect.h/20,
            m_buttonRect.w - m_buttonRect.w/20, m_buttonRect.h - m_buttonRect.h/20);
    m_textInstanceExists = true;
}

Button::Button(SDL_Renderer * renderer, const MOUSE_LISTENER_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[BUTTON_STATES_NUMBER])
: m_buttonRect(buttonRect),m_buttonId(buttonId), m_renderer(renderer)
{   
    m_buttonImage[NO_ACTION] = buttonImage[NO_ACTION];
    m_buttonImage[MOUSE_OVER] = buttonImage[MOUSE_OVER];
    m_buttonImage[PRESSED] = buttonImage[PRESSED];       
    m_buttonState = NO_ACTION;
    m_textInstanceExists = false;
}

Button::~Button() {

    if (m_textInstanceExists)
    delete m_buttonText;
    
}

bool Button::detectSelection(int x, int y) const {
    return (x > m_buttonRect.x && x < m_buttonRect.x + m_buttonRect.w && y > m_buttonRect.y && y < m_buttonRect.y + m_buttonRect.h );
}

void Button::draw() const {
    SDL_RenderCopy(m_renderer, m_buttonImage[m_buttonState], NULL, &m_buttonRect);
    if (m_textInstanceExists)
    m_buttonText->draw();
}

void Button::onClickDown(SDL_Event * evt) {
    setButtonState(PRESSED);
    
}

void Button::onClickUp(SDL_Event * evt) {
    setButtonState(MOUSE_OVER);
}

void Button::onMouseMotion(SDL_Event* evt) {
        setButtonState(MOUSE_OVER);
}

void Button::noFocus() {
    setButtonState(NO_ACTION);
}

void Button::setText(std::string& buttonText, TTF_Font * font) {
    if (!m_textInstanceExists)
    {
        m_buttonText = new Text(m_renderer, font, buttonText, Colors::st_white, m_buttonRect.x + m_buttonRect.w/40, m_buttonRect.y + m_buttonRect.h/20,m_buttonRect.w - m_buttonRect.w/20, m_buttonRect.h - m_buttonRect.h/20);
        m_textInstanceExists = true;
    }
    else
        cerr << "Button : " << m_buttonId << " text already exists" << endl;
    
}



/* 
 * File:   Button.cpp
 * Author: Sylvain
 * 
 * Created on 8 avril 2017, 12:16
 */

#include "Button.h"

using namespace std;

Button::Button(SDL_Renderer * renderer, Text * buttonText, const BUTTON_ID buttonId, const SDL_Rect & buttonRect, SDL_Texture * buttonImage[3])
: m_buttonRect(buttonRect),m_buttonId(buttonId), m_renderer(renderer), m_buttonText(buttonText)
 {   
    m_buttonImage[NO_ACTION] = buttonImage[NO_ACTION];
    m_buttonImage[MOUSE_OVER] = buttonImage[MOUSE_OVER];
    m_buttonImage[PRESSED] = buttonImage[PRESSED];       
    m_buttonState = NO_ACTION;
}

Button::~Button() {
    delete m_buttonText;
}

bool Button::detectSelection(int x, int y) const {
    return (x > m_buttonRect.x && x < m_buttonRect.x + m_buttonRect.w && y > m_buttonRect.y && y < m_buttonRect.y + m_buttonRect.h );
}

void Button::draw() const {
    SDL_RenderCopy(m_renderer, m_buttonImage[m_buttonState], NULL, &m_buttonRect);
    m_buttonText->draw();
}





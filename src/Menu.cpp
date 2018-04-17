/* 
 * File:   Menu.cpp
 * Author: Sylvain
 * 
 * Created on 9 avril 2017, 11:57
 */

#include "Menu.h"
#include "Text.h"
#include <iostream>


using namespace std;


Menu::Menu(SDL_Renderer * renderer): m_menuTexture(NULL), m_renderer(renderer){
}

void Menu::setMenuTexture(SDL_Texture* menuTexture) {
    if (NULL == menuTexture)
        cout << "Error charging menu graphics" << endl;
    m_menuTexture = menuTexture;
}

Menu::~Menu() {    
    vector<IDrawable*>::const_iterator it;
    for (it = m_drawableVector.begin(); it != m_drawableVector.end(); ++it) {
        delete (*it);
    }
        
    if (NULL != m_menuTexture)
    SDL_DestroyTexture(m_menuTexture);
}



void Menu::draw() const {
    if (NULL != m_menuTexture)
    SDL_RenderCopy(m_renderer, this->m_menuTexture, NULL, NULL);
    
    vector<IDrawable*>::const_iterator it;
    for (it = m_drawableVector.begin(); it != m_drawableVector.end(); ++it) {
        (*it)->draw();
    }
    SDL_RenderPresent(m_renderer);
}



Text * Menu::createText(TTF_Font* font, const char* text, const Color& color, float fx, float fy, float fw, float fh) {
    string textString(text);
    Text * createdText = new Text(m_renderer, font, textString , color, (int) (fx*XU), (int) (fy*YU), (int) (fw*XU), (int) (fh*YU));
    m_drawableVector.push_back(createdText);
    return createdText;
}

Button * Menu::createButton(const MOUSE_LISTENER_ID buttonId, SDL_Texture* buttonImage[], TTF_Font* font, const char* text, const Color& color, float fx, float fy, float fw, float fh) {
    SDL_Rect buttonRect = {(int) (fx*XU), (int) (fy*YU), (int) (fw*XU), (int) (fh*YU)};
    string buttonString(text);
    Button * createdButton = new Button(m_renderer, buttonId, buttonRect, buttonImage);
    createdButton->setText(buttonString, font);
    
    m_drawableVector.push_back(createdButton);
    m_mouseListenerVector.push_back(createdButton);
    return createdButton;
}

Ball* Menu::createBall(const char* textureFileName) {
    Ball * createdBall = new Ball(m_renderer, textureFileName);
    m_drawableVector.push_back(createdBall);
    m_keyboardListenerVector.push_back(createdBall);
    return createdBall;
}

Racket* Menu::createRacket(int x, int y, T_ORIENTATION direction, const Color& fillColor, const Color& borderColor,
        T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition) {
    Racket * createdRacket = new Racket(m_renderer, x, y, direction, fillColor, borderColor, firstBind, secondBind, racketPosition);
    m_drawableVector.push_back(createdRacket);
    m_keyboardListenerVector.push_back(createdRacket);
    return createdRacket;
}

RotatingRacket* Menu::createRotatingRacket(int x, int y, const Color& fillColor, const Color& borderColor,
T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition) {
    RotatingRacket * createdRotatingRacket = new RotatingRacket(m_renderer, x, y, fillColor, borderColor, firstBind, secondBind, racketPosition);
    m_drawableVector.push_back(createdRotatingRacket);
    m_keyboardListenerVector.push_back(createdRotatingRacket);
    return createdRotatingRacket;
}

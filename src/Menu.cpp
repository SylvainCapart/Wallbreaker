/* 
 * File:   Menu.cpp
 * Author: Sylvain
 * 
 * Created on 9 avril 2017, 11:57
 */

#include "Menu.h"

using namespace std;

Menu::Menu(SDL_Renderer * renderer):m_renderer(renderer) {
}

void Menu::setMenuTexture(SDL_Texture* menuTexture) {
    if (NULL == menuTexture)
        cout << "Error charging menu graphics" << endl;
    m_menuTexture = menuTexture;
}

Menu::~Menu() {
    SDL_DestroyTexture(m_menuTexture);
    m_buttonVector.clear();
    m_drawableVector.clear();
   
}



void Menu::draw() const {
    vector<IDrawable*>::const_iterator it;
    SDL_RenderCopy(m_renderer, this->m_menuTexture, NULL, NULL);
    for (it = m_drawableVector.begin(); it != m_drawableVector.end(); ++it) {
        (*it)->draw();
    }
    SDL_RenderPresent(m_renderer);
}

std::vector<IDrawable*>& Menu::getDrawableList() {
    return m_drawableVector;
}

std::vector<Button*>& Menu::getButtonList() {
    return m_buttonVector;
}
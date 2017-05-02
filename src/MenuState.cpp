/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MenuState.cpp
 * Author: user
 * 
 * Created on 23 avril 2017, 15:00
 */

#include "MenuState.h"
#include "Game.h"

#include <iostream>

using namespace std;

MenuState::MenuState(SDL_Renderer * renderer, Game & game) : m_renderer(renderer), m_game(game) {
    m_menu = new Menu(renderer);
    SDL_Color white = {255, 255, 255};

    m_titleFont = TTF_OpenFont("font/play the game.ttf", 124);
    if (!m_titleFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }

    m_infoFont = TTF_OpenFont("font/LinLibertine_R.ttf", 64);
    if (!m_infoFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
    m_menu->setMenuTexture(IMG_LoadTexture(renderer, "img/space1.jpg"));

    m_buttonImage[NO_ACTION] = IMG_LoadTexture(renderer, "img/button_no_action.png");
    m_buttonImage[MOUSE_OVER] = IMG_LoadTexture(renderer, "img/button_mouse_over.png");
    m_buttonImage[PRESSED] = IMG_LoadTexture(renderer, "img/button_pressed.png");

    //TODO : replace hard values with a proper solution
    Text * text1 = new Text(renderer, m_titleFont, "Wallbreaker", white, 50, 70, 700, 100);
    Text * text2 = new Text(renderer, m_infoFont, "Player 1 : QZSD", white, 400, 200, 300, 60);
    Text * text3 = new Text(renderer, m_infoFont, "Player 2 : UP DOWN", white, 400, 250, 340, 60);
    Text * text4 = new Text(renderer, m_infoFont, "LEFT RIGHT", white, 400, 310, 170, 60);
    Text * text5 = new Text(renderer, m_infoFont, "SPACE : Launch ball", white, 400, 360, 300, 60);
    Text * text6 = new Text(renderer, m_infoFont, "and middle racket", white, 400, 410, 300, 60);
    Text * text7 = new Text(renderer, m_infoFont, "ESCAPE : Quit", white, 400, 460, 300, 60);

    SDL_Rect startRect = {100, 220, 260, 80};
    SDL_Rect quitRect = {100, 350, 260, 80};
    Text * startText = new Text(renderer, m_infoFont, "Start", white, 100, 220, 250, 80);
    Text * quitText = new Text(renderer, m_infoFont, "Quit", white, 100, 350, 250, 80);
    Button * startButton = new Button(renderer, startText, MENU_START, startRect, m_buttonImage);
    Button * quitButton = new Button(renderer, quitText, MENU_QUIT, quitRect, m_buttonImage);


    m_menu->getDrawableList().push_back(text1);
    m_menu->getDrawableList().push_back(text2);
    m_menu->getDrawableList().push_back(text3);
    m_menu->getDrawableList().push_back(text4);
    m_menu->getDrawableList().push_back(text5);
    m_menu->getDrawableList().push_back(text6);
    m_menu->getDrawableList().push_back(text7);
    m_menu->getDrawableList().push_back(startButton);
    m_menu->getDrawableList().push_back(quitButton);

    m_menu->getButtonList().push_back(startButton);
    m_menu->getButtonList().push_back(quitButton);
}

MenuState::~MenuState() {
    TTF_CloseFont(m_infoFont);
    TTF_CloseFont(m_titleFont);
    SDL_DestroyTexture(m_buttonImage[NO_ACTION]);
    SDL_DestroyTexture(m_buttonImage[MOUSE_OVER]);
    SDL_DestroyTexture(m_buttonImage[PRESSED]);
    delete m_menu;

}

void MenuState::draw() {
    m_menu->draw();
}

void MenuState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;

}

void MenuState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    if (SDLK_ESCAPE == evt->key.keysym.sym) {
        m_game.stop();
    }
}

void MenuState::onQuit() {
    m_game.stop();
}

void MenuState::update() {

}

void MenuState::onEvent(SDL_Event* evt) {
    vector<Button*>::const_iterator it;
    switch (evt->type) {
        case SDL_QUIT: onQuit();
            break;
        case SDL_KEYDOWN: onKeyDown(evt);
            break;
        case SDL_KEYUP: onKeyUp(evt);
            break;
        default:
            break;
    }

    SDL_GetMouseState(&m_mouseX, &m_mouseY);
    for (it = m_menu->getButtonList().begin(); it != m_menu->getButtonList().end(); ++it) {
        if ((*it)->detectSelection(m_mouseX, m_mouseY)) {
            if (MENU_START == (*it)->getDrawableId() && SDL_MOUSEBUTTONUP == evt->type) {
                m_game.setCurrentState(GAME_STATE);
                return;
            } else if (MENU_QUIT == (*it)->getDrawableId() && SDL_MOUSEBUTTONUP == evt->type) {
                m_game.stop();
                return;
            } else if (SDL_MOUSEBUTTONDOWN == evt->type) {
                (*it)->setButtonState(PRESSED);
            } else
                (*it)->setButtonState(MOUSE_OVER);
        } else
            (*it)->setButtonState(NO_ACTION);

    }

}

void MenuState::init() {
    m_mouseX = 0;
    m_mouseY = 0;
}


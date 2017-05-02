/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ScoreState.cpp
 * Author: user
 * 
 * Created on 23 avril 2017, 15:00
 */

#include "ScoreState.h"
#include "Game.h"

#include <iostream>

using namespace std;

ScoreState::ScoreState(SDL_Renderer *renderer, Game & game):m_game(game), m_renderer(renderer) {

    m_infoFont = TTF_OpenFont("font/LinLibertine_R.ttf", 64);
    if (!m_infoFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
    SDL_Color white = {255, 255, 255};
    m_menu = new Menu(renderer);
    m_menu->setMenuTexture(IMG_LoadTexture(renderer, "img/space1.jpg"));
    
    Text * text1 = new Text(renderer, m_infoFont, "Well done ladies and gentlemen !!!", white, 50, 240, 700, 70);
    Text * text2 = new Text(renderer, m_infoFont, "Escape : back to menu", white, 190, 300, 400, 70);
    m_menu->getDrawableList().push_back(text1);
    m_menu->getDrawableList().push_back(text2);
    
}


ScoreState::~ScoreState() {
    delete m_menu;
}

void ScoreState::draw() {
    m_menu->draw();
}

void ScoreState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;
}

void ScoreState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    if (SDLK_ESCAPE == evt->key.keysym.sym) {
        
        m_game.setCurrentState(MENU_STATE);
        
    }
}

void ScoreState::onQuit() {
    m_game.stop();
}

void ScoreState::update() {
    
}

void ScoreState::onEvent(SDL_Event* evt) {
    switch (evt->type) {
        case SDL_QUIT: onQuit();
        break;
        case SDL_KEYDOWN: onKeyDown(evt);
        break;
        case SDL_KEYUP: onKeyUp(evt);
        break;
    }
}

void ScoreState::init() {

}



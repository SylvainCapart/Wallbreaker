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
#include "Const.h"
#include "Types.h"
#include <sstream>

using namespace std;

MenuState::MenuState(SDL_Renderer * renderer, Game & game, StateContext & stateContext) : m_renderer(renderer), m_game(game),
        m_stateContext(stateContext) {
    
    m_menuTable[MAIN_MENU] = new Menu(renderer);
    m_menuTable[LEVEL_CHOICE_MENU] = new Menu(renderer);
    m_menuTable[SCORE_MENU] = new Menu(renderer);
    
    m_menuTable[MAIN_MENU]->setMenuTexture(IMG_LoadTexture(renderer, "img/space1.jpg"));
    
    m_buttonImage[NO_ACTION] = IMG_LoadTexture(renderer, "img/blue_button_no_action.png");
    m_buttonImage[MOUSE_OVER] = IMG_LoadTexture(renderer, "img/blue_button_mouse_over.png");
    m_buttonImage[PRESSED] = IMG_LoadTexture(renderer, "img/blue_button_pressed.png");
    
    int explIndex = 4; //explanation begin index 
    //texts and buttons are placed following the grid defined by XU and YU
    m_menuTable[MAIN_MENU]->createText(Fonts::st_titleFont, "Wallbreaker", Colors::st_white, 0.5 , 0.5, 7, 2 );
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "Player 1 : QZSD", Colors::st_white, 4 , explIndex , 3, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "Player 2 : UP DOWN", Colors::st_white, 4 , (explIndex + 1) , 3.5, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "LEFT RIGHT", Colors::st_white, 4 , (explIndex + 2) , 2, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "SPACE : Launch ball", Colors::st_white, 4 , (explIndex + 3) , 3, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "and middle racket", Colors::st_white, 4 , (explIndex + 4) , 3, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "ESCAPE : Quit", Colors::st_white, 4 , (explIndex + 5) , 3, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_numberFont, "0.4", Colors::st_white, 3.5, 2.8, 1, 1);
    m_menuTable[MAIN_MENU]->createText(Fonts::st_infoFont, "Sylvain Capart 2017", Colors::st_white, 2.5, 10.9, 3, 1);
    
    m_menuTable[MAIN_MENU]->createButton(B_MENU_START, m_buttonImage, Fonts::st_infoFont,
            "Start", Colors::st_white, 1, 5, 2.5, 1.6);
    m_menuTable[MAIN_MENU]->createButton(B_MENU_QUIT, m_buttonImage, Fonts::st_infoFont,
            "Quit", Colors::st_white, 1, 7, 2.5, 1.6);
    
    m_menuTable[LEVEL_CHOICE_MENU]->setMenuTexture(IMG_LoadTexture(renderer, "img/space1.jpg"));

    m_menuTable[LEVEL_CHOICE_MENU]->createButton(B_RANDOM_LEVEL, m_buttonImage, Fonts::st_infoFont,
            "Random level", Colors::st_white, 2.5, 3, 3, 1.6);
    m_menuTable[LEVEL_CHOICE_MENU]->createButton(B_DEMO_LEVEL, m_buttonImage, Fonts::st_infoFont,
            "Demo level", Colors::st_white, 2.5, 5, 3, 1.6);
    m_menuTable[LEVEL_CHOICE_MENU]->createButton(B_EDIT_LEVEL, m_buttonImage, Fonts::st_infoFont,
            "Create level !", Colors::st_white, 2.5, 7, 3, 1.6);
    m_menuTable[LEVEL_CHOICE_MENU]->createButton(B_BACK_FROM_LEVEL_CHOICE, m_buttonImage, Fonts::st_arrowFont,
            "B", Colors::st_white, 0.5, 9, 1, 0.8);
    
    m_menuTable[SCORE_MENU]->setMenuTexture(IMG_LoadTexture(renderer, "img/space1.jpg"));
    
    m_menuTable[SCORE_MENU]->createButton(B_BACK_FROM_LEVEL_CHOICE, m_buttonImage, Fonts::st_arrowFont,
            "B", Colors::st_white, 0.5, 9, 1, 0.8);

    m_menuTable[SCORE_MENU]->createText(Fonts::st_infoFont, "Well done ladies and gentlemen !", Colors::st_white, 0.5, 4, 7, 1.5);
    m_menuTable[SCORE_MENU]->createText(Fonts::st_infoFont, "Score : ", Colors::st_white, 2.3, 6.4, 1.5, 1.4);
    
    m_scoreText = m_menuTable[SCORE_MENU]->createText(Fonts::st_numberFont, to_string(0).c_str(),
            Colors::st_white, 3.8, 6.7, 0.6, 1);
    
}

MenuState::~MenuState() {
 
    SDL_DestroyTexture(m_buttonImage[NO_ACTION]);
    SDL_DestroyTexture(m_buttonImage[MOUSE_OVER]);
    SDL_DestroyTexture(m_buttonImage[PRESSED]);
    for (int i = 0; i < MENU_STATE_NUMBER; i++)
        delete m_menuTable[i];
    
    
}

void MenuState::draw() {
    
    m_currentMenuState->draw();
}

void MenuState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;
    
}

void MenuState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    if (SDLK_ESCAPE == evt->key.keysym.sym && MAIN_MENU == m_currentMenuStateId) {
        onQuit();
    }
    if (SDLK_ESCAPE == evt->key.keysym.sym && LEVEL_CHOICE_MENU == m_currentMenuStateId) {
        setCurrentMenuState(MAIN_MENU);
    }
}

void MenuState::onQuit() {
    m_game.stop();
}

void MenuState::onEvent(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
    SDL_GetMouseState(&m_mouseX, &m_mouseY);
    
    for (it = m_currentMenuState->getMouseListenerVector().begin(); it != m_currentMenuState->getMouseListenerVector().end(); ++it) 
                    (*it)->noFocus();
    
    switch (evt->type) {
        case SDL_QUIT: onQuit();
        break;
        case SDL_KEYDOWN: onKeyDown(evt);
        break;
        case SDL_KEYUP: onKeyUp(evt);
        break;
        case SDL_MOUSEBUTTONUP: 
            onClickUp(evt);
            break;
        case SDL_MOUSEBUTTONDOWN : 
            onClickDown(evt);
            break;
        case SDL_MOUSEMOTION : 
            onMouseMotion(evt);
            break;
        default:
            break;
    }
}


void MenuState::onClickDown(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
    for (it = m_currentMenuState->getMouseListenerVector().begin(); it != m_currentMenuState->getMouseListenerVector().end(); ++it) 
            {
                if ((*it)->detectSelection(m_mouseX, m_mouseY))
                    (*it)->onClickDown(evt);
            }
}

void MenuState::onClickUp(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
for (it = m_currentMenuState->getMouseListenerVector().begin(); it != m_currentMenuState->getMouseListenerVector().end(); ++it)
            {   
                if ((*it)->detectSelection(m_mouseX, m_mouseY))
                {
                    (*it)->onClickUp(evt);
                    if (B_MENU_START == (*it)->getMouseListenerId())
                    {
                        setCurrentMenuState(LEVEL_CHOICE_MENU);
                        return;
                    }
                    else if (B_MENU_QUIT == (*it)->getMouseListenerId())
                    {
                        m_game.stop();
                        return;
                    }
                    else if (B_RANDOM_LEVEL == (*it)->getMouseListenerId())
                    {
                        m_stateContext.setLoadLevelType(RANDOM_LEVEL);
                        m_game.setCurrentState(GAME_STATE);
                        return;
                    } 
                    else if (B_DEMO_LEVEL == (*it)->getMouseListenerId())
                    {
                        m_stateContext.setLoadLevelType(DEMO_LEVEL);
                        m_game.setCurrentState(GAME_STATE);
                        return;
                    } 
                    else if (B_EDIT_LEVEL == (*it)->getMouseListenerId())
                    {
                        
                        m_game.setCurrentState(EDIT_STATE);
                        return;
                    } 
                    else if (B_BACK_FROM_LEVEL_CHOICE == (*it)->getMouseListenerId())
                    {
                        setCurrentMenuState(MAIN_MENU);
                        return;
                    }
                    else
                        cout << "Unknown button has been detected and clicked : check MenuState.cpp:onEvent()"; //should not happen   
                }  
            }
}

void MenuState::onMouseMotion(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
    for (it = m_currentMenuState->getMouseListenerVector().begin(); it != m_currentMenuState->getMouseListenerVector().end(); ++it) 
    {
        if ((*it)->detectSelection(m_mouseX, m_mouseY))
            (*it)->onMouseMotion(evt);
    }
}


void MenuState::init() {
    m_mouseX = 0;
    m_mouseY = 0;
    if (MAIN_MENU == m_stateContext.getMenuType())
        setCurrentMenuState(MAIN_MENU);
    else if (LEVEL_CHOICE_MENU == m_stateContext.getMenuType())
        setCurrentMenuState(LEVEL_CHOICE_MENU);
    else // score_menu
    {
        m_scoreText->changeText(to_string(m_stateContext.getScore()));
        setCurrentMenuState(SCORE_MENU);
    }
    
}

void MenuState::setCurrentMenuState(T_MENU_STATE newState) {
    m_currentMenuState = m_menuTable[newState];
    m_currentMenuStateId = newState;
}

string MenuState::to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
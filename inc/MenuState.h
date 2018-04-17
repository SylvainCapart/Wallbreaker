/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MenuState.h
 * Author: user
 *
 * Created on 23 avril 2017, 15:00
 */

#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "IState.h"
#include "IDrawable.h"
#include "StateContext.h"
#include "Menu.h"
#include <map> 

class Game;

class MenuState : public IState{
public:
    /// 
    /// \param renderer : renderer where we draw graphics
    /// \param game : reference to the class holding the IStates
    MenuState( SDL_Renderer * renderer, Game & game, StateContext & stateContext);
    virtual ~MenuState();
    void draw();
    void onKeyDown(SDL_Event* evt) ;
    void onKeyUp(SDL_Event* evt);
    void onQuit();
    void onEvent(SDL_Event* evt);
    void init();
    void setCurrentMenuState(T_MENU_STATE newState);
    void onClickDown(SDL_Event* evt);
    void onClickUp(SDL_Event* evt);
    void onMouseMotion(SDL_Event* evt);
    std::string to_string(int i);

private:
    SDL_Renderer * m_renderer; //renderer where we draw graphics
    SDL_Texture * m_buttonImage[3]; //different images of the buttons. The table is created in the state 
    // and passed to the menu as argument in the constructor
    Game & m_game; // reference to the class holding the IStates
    StateContext & m_stateContext;
    int m_mouseX, m_mouseY; // position of the mouse
    Menu * m_menuTable[MENU_STATE_NUMBER]; //different menu states
    Menu * m_currentMenuState;
    Text * m_scoreText;
    T_MENU_STATE m_currentMenuStateId;
    std::map<int,int> m_keys; // map of keys to know which one is up or done

};

#endif /* MENUSTATE_H */


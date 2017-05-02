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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Menu.h"
#include <map> 
#include <list>
#include <iostream>
#include <string>
#include "Const.h"
#include "Types.h"

class Game;

class MenuState : public IState {
public:
    /// 
    /// \param renderer : renderer where we draw graphics
    /// \param game : reference to the class holding the IStates
    MenuState( SDL_Renderer * renderer, Game & game);

    virtual ~MenuState();
    void draw();
    void onKeyDown(SDL_Event* evt) ;
    void onKeyUp(SDL_Event* evt);
    void onQuit();
    void onEvent(SDL_Event* evt);
    void update();
    void init();
    
private:
    SDL_Renderer * m_renderer; //renderer where we draw graphics
    SDL_Texture * m_buttonImage[3]; //different images of the buttons. The table is created in the state 
    // and passed to the menu as argument in the constructor
    Game & m_game; // reference to the class holding the IStates
    int m_mouseX, m_mouseY; // position of the mouse
    Menu * m_menu; //menu holding the different drawables, added in the constructor of MenuState
    std::map<int,int> m_keys; // map of keys to know which one is up or done
    TTF_Font* m_titleFont; //font of the title
    TTF_Font* m_infoFont; // font of the other text messages
};

#endif /* MENUSTATE_H */


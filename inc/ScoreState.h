/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ScoreState.h
 * Author: user
 *
 * Created on 23 avril 2017, 15:00
 */

#ifndef SCORESTATE_H
#define SCORESTATE_H

#include "IState.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>  
#include "Menu.h"

class Game;

class ScoreState : public IState {
    public:
        ScoreState(SDL_Renderer * renderer, Game & game);
        virtual ~ScoreState();
        void draw();
        void onKeyDown(SDL_Event* evt);
        void onKeyUp(SDL_Event* evt);
        void onQuit();
        void onEvent(SDL_Event* evt);
        void update();
        void init();

    private:
        Game & m_game; //reference to the class holding the IState
        Menu * m_menu; //menu allowing to add then draw the IDrawable
        TTF_Font * m_infoFont; //text font
        std::map<int,int> m_keys; // map of keys to know which one is up or done
        SDL_Renderer * m_renderer; //renderer where we draw graphics
            
};

#endif /* SCORESTATE_H */


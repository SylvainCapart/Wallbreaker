/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EditState.h
 * Author: user
 *
 * Created on 2 juillet 2017, 18:16
 */

#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "Game.h"
#include "Brick.h"
#include "StateContext.h"
#include "Menu.h"
#include "IState.h"
#include "IMouseListener.h"
#include <SDL.h>
#include <map>


class EditState : public IState{
public:
    EditState(SDL_Renderer *renderer, Game & game, StateContext & stateContext);
    virtual ~EditState();
    void draw() ;
    void init() ;
    void onClickDown(SDL_Event* evt) ;
    void onClickUp(SDL_Event* evt) ;
    void onKeyDown(SDL_Event* evt);
    void onKeyUp(SDL_Event* evt);
    void onEvent(SDL_Event* evt) ;
    void onMouseMotion(SDL_Event* evt) ;
    void onQuit() ;


    /// \goal : change brush state and color of the example brick
    void changeBrush();
    
    /// \goal : set the resistance of the brick where the mouse if placed. It must be
    /// inside the drawing area and not on the middle racket area
    void paint();
    
     /// \goal : clear the brick map
    void clearBrickMap();
    
     /// \goal : detect if the mouse is on the brick area
    bool detectMouseOnBrickArea();
    
     /// \goal : draw the brick map
    void buildBrickMap();
    
    std::string to_string(int i);
    
     /// \goal : load a level (series of numbers in a txt file) into the map of bricks
    bool loadFileLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string & fileName);
    
    /// \goal : save a level (series of numbers in a txt file) from the map of bricks
    bool saveFileLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string & fileName);
    
private:
    SDL_Renderer * m_renderer; //renderer where we draw graphics
    Game & m_game; // reference to the class holding the IStates
    StateContext & m_stateContext;
    SDL_Texture * m_buttonImage[3]; //different images of the buttons. The table is created in the state 
    // and passed to the menu as argument in the constructor
    Menu * m_menu;
    Brick * m_brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT];
    int m_mouseX, m_mouseY; // position of the mouse
    std::map<int,int> m_keys; // map of keys to know which one is up or done

    Brick * m_exampleBrick; // example brick to show what the user is going to paint
    int m_currentBrushState; // state of the example brick aka its resistance => its color
    
    bool m_activatedBrush; // if it is possible to paint (place a brick) or not

};

#endif /* EDITSTATE_H */


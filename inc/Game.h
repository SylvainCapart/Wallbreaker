/* 
 * File:   Game.h
 * Author: Sylvain
 * 
 * Main class of the program, handling gameplay, menus, graphics.
 * List of TODO in .cpp
 *
 * Created on 5 janvier 2017, 10:16
 */

#ifndef GAME_H
#define	GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cstdio>   
#include <iostream>
#include <string>
#include "Const.h"
#include "Types.h"
#include "IState.h"
#include "GameState.h"
#include "MenuState.h"
#include "ScoreState.h"
#include <assert.h>



// See Game.cpp for functionnal description

class Game {
public:
    Game();  
    virtual ~Game(); 
    
    /// \goal : launches the main thread, run()
    void start(); 
    
    /// \goal : stop the main thread, run()
    void stop() ; 
    
    /// \param fps : frames per second
    /// \goal : displays the title and the fps
    void fpsChanged( int fps );  
    
    /// \goal : main thread, handling current state, sending events and draw orders to this state
    void run();
    
    /// \goal : change the current state
    void setCurrentState(STATE newState);
    
private:  
    int m_frameSkip ;  // redraw graphics once enough frames have passed
    int m_running ;  // main thread activator
    IState * m_currentState; // current selected state among m_stateVector 
    IState * m_stateVector[STATE_NUMBER]; // table of IState*, holding all possible states of the game
    SDL_Window * m_window; // window needed to support graphics creation
    SDL_Renderer * m_renderer; // renderer where we draw graphics

    
};  

#endif	/* GAME_H */


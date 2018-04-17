/* 
 * File:   Game.cpp
 * Author: Sylvain
 * 
 * Main class of the program, handling gameplay, menus, graphics.
 * 
 * 
 * Created on 5 janvier 2017, 10:16
 */

#include "Game.h"
#include "GameState.h"
#include "MenuState.h"
#include "EditState.h"
#include "Fonts.h"
#include "Const.h"
#include "Types.h"
#include <SDL_image.h>
#include <cstdio>   
#include <iostream>
#include <string>

using namespace std;

Game::Game()
: m_frameSkip(0), m_running(0), m_window(NULL), m_renderer(NULL) {
    int flags = SDL_WINDOW_SHOWN;
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS)) {
        cout << "Error using SDL_Init()" << endl;
        return;
    }
    
    if (TTF_Init() < 0) {
        cout << "Error using TTF_Init()" << endl;
        return;
    }
    Fonts::openAllFonts();
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &m_window, &m_renderer)) {
        return;
    }
    
    m_stateVector[MENU_STATE] = new MenuState(m_renderer, *this, m_stateContext);
    m_stateVector[GAME_STATE] = new GameState(m_renderer, *this,  m_stateContext);
    m_stateVector[EDIT_STATE] = new EditState(m_renderer, *this,  m_stateContext);
    setCurrentState(MENU_STATE);
    
}

Game::~Game() {
    
    for (int i = 0; i < STATE_NUMBER; ++i) {
        delete m_stateVector[i];
        m_stateVector[i] = NULL;
    }
    
    if (NULL != m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = NULL;
    }
    
    if (NULL != m_window) {
        SDL_DestroyWindow(m_window);
        m_window = NULL;
    }
    Fonts::closeAllFonts();
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}



/* Function : start
 * Input : none
 * Output : none
 * Goal : launches the main thread
 
 */

void Game::start() {
    this->m_running = 1;
    run();
}

void Game::stop() {
    m_running = 0;
}

void Game::fpsChanged(int fps) {
    char szFps[ 128 ];
    sprintf(szFps, "%s: %d FPS", "SDL2 Base C++ - Use Arrow Keys / ZQSD SPACE to Move", fps);
    SDL_SetWindowTitle(m_window, szFps);
}

/* Function : run
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : main thread, handling drawings following game state.
 * 
 * TODO : optimisation
 * decorrelate from the drawing and collision activities
 */

void Game::run() {
    int past = SDL_GetTicks();
    int now = past, pastFps = past;
    int fps = 0, framesSkipped = 0;
    
    SDL_Event event;
    while (m_running) {
        //speedtest__("Running test speed : ") {
        int timeElapsed = 0;
        if (SDL_PollEvent(&event)) {
            m_currentState->onEvent(&event);
        }
        
        timeElapsed = (now = SDL_GetTicks()) - past;
        
        if (timeElapsed >= UPDATE_INTERVAL) {
            past = now;
            
            if (framesSkipped++ >= m_frameSkip) {
                m_currentState->draw();
                ++fps;
                framesSkipped = 0;
            }
        }
        if (now - pastFps >= 1000) {
            pastFps = now;
            fpsChanged(fps);
            fps = 0;
        }
        
        SDL_Delay(1);
        
    }
    
    stop();
}


void Game::setCurrentState(STATE newState) {
    
    m_currentState = m_stateVector[newState];
    m_currentState->init();
}

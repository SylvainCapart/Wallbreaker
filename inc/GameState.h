/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameState.h
 * Author: user
 *
 * Created on 23 avril 2017, 14:50
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "IState.h"
#include "Ball.h"
#include "Racket.h"
#include "Const.h"
#include "Types.h"
#include "Menu.h"
#include "IDrawable.h"
#include "Text.h"
#include "Game.h"
#include "Brick.h"
#include "StateContext.h"
#include <map>  

class Game;

class GameState : public IState{
public:
    GameState(SDL_Renderer *renderer, Game & game, StateContext & stateContext);

    virtual ~GameState();

    /// \goal : draw all IDrawable in the state
    void draw();
    void onEvent(SDL_Event* evt);
    void onKeyDown(SDL_Event* evt);
    void onKeyUp(SDL_Event* evt);
    void onQuit();
    void init();

    /// \goal : check if the resistance of the bricks is not equal to 0 for all the bricks.
    /// If it is the case, change the state.
    void checkBrickPresence();

    /// \goal : draw the map of bricks
    void buildBrickMap();
    
    /// \goal : get a ratio corresponding to [AC]/[AB]
    float getRacketContactRatio(const Point & A, const Point & B, const Point & C);
    
    /// \goal : get an arbitrary coeff following contactRatio
    int getSpeedModifCoeffFromContactRatio(float contactRatio);
    
    /// \goal : load a random level into the map of bricks
    void  loadRandomLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT]);
    
    /// \goal : load a level (series of numbers in a txt file) into the map of bricks
    bool  loadFileLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string & fileName);

    /// \param renderer : renderer where we draw graphics
    /// \param color : color of the background
    /// \goal : set background color
    void setRenderDrawColor(SDL_Renderer * renderer, Color * color);
    
    /// \param xa, ya : point A coordinates
    /// \param xb, yb : point B coordinates
    /// \goal : return D², square distance between A and B
    int getIntSqrDistance(int xa, int ya, int xb, int yb);
    
    /// \goal : set resistances of the brickMap to 0
    void clearBrickMap();
    
    std::string to_string(int i);
    Racket * getProperRacket(SDL_Keycode keyId);
    void updateRacketsPositions();
    
    /// \returns : the index of the closest racket to the ball
    /// \goal : in order to avoid detecting collision with the other rackets
    T_RACKET_POSITION getClosestRacket() ;
    


private:
    SDL_Renderer * m_renderer; // renderer where we draw the graphics
    Game & m_game; // reference to the class that holds the state 
    std::map<int,int> m_keys; // map of keys to know which one is up or done
    Racket * m_rackets[TOTAL_RACKET_NUMBER]; // table of all rackets
    
    StateContext & m_stateContext; // state context is used to pass load type, score and menu type between states

    Ball * m_ball; // one ball for the actual version
    Brick * m_brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT]; // 2D table containing O 1 or 2
    T_RACKET_POSITION m_initialRacketId; // initial racket where the ball is placed
    bool m_middleRacketPresent; // mode with middle racket or not. Not used yet.
    bool m_levelSelected; // level has been chosen or not
    bool m_won; // all bricks have been hit or not
    Menu * m_menu;
    unsigned int m_score;
    int32_t m_past;
    unsigned int m_pastScore;
    Text * m_scoreText;
    Color * m_screenColor;
    int m_blockCounter; // to prevent a ball from being blocked : after 3 hits with the x speed or y speed
    // near 0, modify a bit the speed vector of the ball
};

#endif /* GAMESTATE_H */


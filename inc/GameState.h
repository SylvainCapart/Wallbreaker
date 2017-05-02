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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cstdio>  
#include <map>  
#include <list>
#include <cmath>
#include <iostream>
#include <string>
#include "Ball.h"
#include "Racket.h"
#include "Const.h"
#include "Types.h"
#include "IState.h"
#include "IDrawable.h"
#include "GameState.h"
#include "MenuState.h"
#include "ScoreState.h"
#include "Brick.h"
#include "Collision.h"
#include <assert.h>

class Game;

class GameState : public IState{
public:
    GameState(SDL_Renderer *renderer, Game & game);

    virtual ~GameState();

    /// \goal : draw all IDrawable in the state
    void draw();
    void onEvent(SDL_Event* evt);
    void onKeyDown(SDL_Event* evt);
    void onKeyUp(SDL_Event* evt);
    void onQuit();
    /// \goal : update the screen following the keys pressed
    void update();
    void init();

    /// \param keyId : ID of the key pressed
    /// \return : the racket corresponding to the key pressed
    Racket * getProperRacket(SDL_Keycode keyId);
    
    /// \goal : check if the resistance of the bricks is not equal to 0 for all the bricks.
    /// If it is the case, change the state.
    void checkBrickPresence();
    
    /// \goal : place the ball on a random racket
    void placeBall();
    
    /// \goal : fill a rectangle with a color
    void fillRectGame(SDL_Rect* rc, int r, int g, int b );  
    void fillRectGame(SDL_Rect* rc, const Color& color); 
    
    /// \goal : draw the map of bricks
    void buildBrickMap();
    
    /// \goal : get a ratio corresponding to [AC]/[AB]
    float getRacketContactRatio(const Point & A, const Point & B, const Point & C);
    
    /// \goal : get an arbitrary coeff following contactRatio
    int getSpeedModifCoeffFromContactRatio(float contactRatio);
    
    /// \goal : load a level (series of 0,1 or 2 in a txt file) into the map of bricks
    int  loadLevel(Brick * brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]);
    
    /// \param renderer : renderer where we draw graphics
    /// \param color : color of the background
    /// \goal : set background color
    void setRenderDrawColor(SDL_Renderer * renderer, Color * color);
    

private:
    int m_levelId; // id of the level we want to load
    SDL_Renderer * m_renderer; // renderer where we draw the graphics
    Game & m_game; // reference to the class that holds the state 
    std::map<int,int> m_keys; // map of keys to know which one is up or done
    Racket * m_rackets[TOTAL_RACKET_NUMBER]; // table of all rackets
    SDL_Texture * m_ballTexture; // texture of the ball (its image)

    Ball * m_ball; // one ball for the actual version
    Brick * m_brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]; // 2D table containing O 1 or 2
    int m_initialRacketId; // initial racket where the ball is placed
    int m_middleRacketAngle; // angle of rotation for the middle racket
    bool m_middleRacketPresent; // mode with middle racket or not
    bool m_middleRacketRotationActivated; // middle racket rotation activation or inhibition 
    bool m_collisionActivated; // ball collision activated or not
    bool m_levelSelected; // level has been chosen or not
    bool m_won; // all bricks have been hit or not
    
    Color * m_cobalt;
    Color * m_red;
    Color * m_blueGreen;
    Color * m_darkBlueGreen;
    Color * m_yellow;
    Color * m_paleOrange;
    Color * m_green;
    Color * m_black;
    Color * m_darkCobalt;
};

#endif /* GAMESTATE_H */


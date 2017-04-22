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
#include <map>  
#include <list>
#include <cmath>
#include <iostream>
#include <string>
#include "Ball.h"
#include "Racket.h"
#include "Const.h"
#include "Types.h"
#include "Brick.h"
#include "Collision.h"
#include "Level.h"
#include <assert.h>
#include "Menu.h"

// See Game.cpp for functionnal description

class Game {
public:
    Game();  
    Game(const Game& orig);
    virtual ~Game();  
    void start();  
    void stop() ;  
    void draw(); 
    void drawMenu();
    void drawWon();
    
    void checkBrickPresence();
    void initRackets();
    void placeBall();
    void fillRectGame(SDL_Rect* rc, int r, int g, int b );  
    void fillRectGame(SDL_Rect* rc, const Color& color);  
    void fpsChanged( int fps );  
    void onQuit();  
    void onKeyDown( SDL_Event* event );  
    void onKeyUp( SDL_Event* event ); 
    void run();
    void buildBrickMap();
    float getRacketContactRatio(const Point & A, const Point & B, const Point & C);
    int getSpeedModifCoeffFromContactRatio(float contactRatio);

    void setRenderDrawColor(SDL_Renderer * renderer, Color * color);
    void update();
    void notLaunchedBallUpdate();
    Racket * getProperRacket(SDL_Keycode keyId);
    
private:  
    std::map<int,int> keys; // map of keys to know which one is up or done 
    int frameSkip ;  // redraw graphics once enough frames have passed
    int running ;  // main thread activator
    
    // graphics variables
    SDL_Window * window;
    SDL_Renderer * renderer;
    Menu * menu;
    Color * cobalt;
    Color * red;
    Color * blueGreen;
    Color * darkBlueGreen;
    Color * yellow;
    Color * paleOrange;
    Color * green;
    Color * black;
    Color * darkCobalt;
    SDL_Texture * ballTexture;

    SDL_Texture * wonTexture; // end screen graphics
    
    Racket * rackets[TOTAL_RACKET_NUMBER]; // all rackets
  
    Ball * ball; // one ball for the actual version
    
    int initialRacketId; // initial racket where the ball is placed
    int rackets_number; // number of rackets
    int middleRacketAngle; // angle of rotation for the middle racket
    int collisionInhibition; // used to prevent the collision function for happening
    
    
    Brick * brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]; // 2D table containing O 1 or 2
    
    GAMESTATE gameState; // state of the game : MENU, GAME or WON
    
    bool menuDrawn; // menu is present or not
    bool wonDrawn; // end screen is present or not
    bool levelSelected; // level has been chosen or not
    bool won; // all bricks have been hit or not
    bool middleRacketPresent; // mode with middle racket or not
    bool middleRacketRotationActivated; // middle racket rotation activation or inhibition 
    bool collisionActivated; // ball collision activated or not
    bool mouseIsMoving; // to notice mouse movement on screen
    int mouseX, mouseY; // mouse coordinates
    bool mouseButtonDown, mouseButtonUp;
    
    Level * currentLevel; // instance of Level class to use its method, because the methods are not static.
};  

#endif	/* GAME_H */


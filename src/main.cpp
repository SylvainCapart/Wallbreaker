/* 
 * File:   main.cpp
 * Author: Sylvain
 *
 * DONE v0.2 : 
 * Create a proper Menu class, that can handle a proper menu, with choices 
 * highlighted, nice graphics, etc. Former menu (and end screen) were just images.
 * 
 * DONE v0.2 : for each state, implement a vector of IDrawable 
 * 
 * TODO v0.3 : feature
 * Modify the Ball * ball variable or add another variable (may be a List<Ball*>)
 * to handle a multiple ball effect.
 * 
 * DONE v0.1 : feature
 * Replace the actual brickMap containing ints by a proper brickMap that is a 
 * 2D table containing Brick classes.
 * 
 * TODO v0.3 : feature
 * Implement a score/lives system.
 * 
 * DONE v0.2 : implement the pattern state for Game
 * 
 * DONE v0.2 : implement virtual interface IDrawable
 * 
 * TODO v0.3 : add boost smart ptr
 * 
 * DONE v0.2 : minor corrections : hard values, ++i, m_attributes, check switch cases, reverse ==
 * 
 * TODO v0.X : optimisation
 * Decorrelate the run, draw and collision activities in different threads. For
 * moment they are all in the run() thread.
 * 
 * DONE v0.2 : handle bugs occuring when the ball and the middle racket collide : ball disappears.
 * 
 * DONE v0.1 : debug
 * Handle a bug causing the ball to perform multiple hits on the same bricks, or 
 * to perform several hits on different bricks.
 * 
 * TODO v0.3 : debug
 * Handle a bug causing the ball to not escape the middle racket when the player
 * make it rotate and a collision occurs. Therefore the ball after the rebound, 
 * is considered to be still in collision with the segment and a new normal vector 
 * is chosen towards the inside of the racket. The ball escapes when it reaches the 
 * corner.
 * 
 * DONE v0.1 : debug
 * Handle a bug causing the end screen to be displayed when not needed.
 * 
 * TODO v0.3 : improvement
 * Handle ball collision on the sides of the rackets, which causes the ball to 
 * enter inside the racket.
 * 
 * TODO v0.3 : feature
 * Implement an edit mode, save and load different levels
 * 
 * Created on 29 décembre 2016, 10:03
 */

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Const.h"
#include "Types.h"
#include "Game.h"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));
    Game game;
    game.start();
  
}

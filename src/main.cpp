/* 
 * File:   main.cpp
 * Author: Sylvain
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

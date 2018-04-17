/* 
 * File:   constantes.h
 * Author: Sylvain
 *
 * Constants of the projects. 
 * 
 * Created on 3 janvier 2017, 09:53
 */



#ifndef CONSTANTES_H
#define	CONSTANTES_H

#include "Types.h"
#include <time.h>
#include <ctime>


#ifdef	__cplusplus
extern "C" {
#endif



#ifndef speedtest__             
#define speedtest__(data)   for (long blockTime = 0; (blockTime == 0 ? (blockTime = clock()) != 0 : false); printf(data "%.9fs\n", (double) (clock() - blockTime) / CLOCKS_PER_SEC))
#endif

    //General
#define STATE_NUMBER 3
#define MENU_STATE_NUMBER 3
    
    // Size of a brick
#define BRICK_WIDTH 40
#define BRICK_HEIGHT 20
    
    // Number of bricks in the window
#define NB_BRICKS_WIDTH 20
#define NB_BRICKS_HEIGHT 30
    
    // Size of the window defined regarding the number of bricks
#define WINDOW_WIDTH BRICK_WIDTH*NB_BRICKS_WIDTH
#define WINDOW_HEIGHT BRICK_HEIGHT*NB_BRICKS_HEIGHT
    
    // Defining an area where bricks are allowed

#define BRICK_AREA_X_NUMBER 14 // Width of this area, in number of bricks. Max  = 16.
#define BRICK_AREA_Y_NUMBER 20// Height of the area, in number of bricks. Max = 24.
#define BRICK_AREA_X_ORIGIN_NUMBER (NB_BRICKS_WIDTH - BRICK_AREA_X_NUMBER)/2  // X component of the area. Min = 1.
#define BRICK_AREA_Y_ORIGIN_NUMBER (NB_BRICKS_HEIGHT - BRICK_AREA_Y_NUMBER)/2 // Y component of the area. Min = 1.

    // Rackets constants
#define RACKET_WIDTH 80
#define RACKET_HEIGHT 20
#define BASE_RACKET_NUMBER (POSITION_NB_ -1)
#define TOTAL_RACKET_NUMBER POSITION_NB_
#define MARGIN_BOTTOM_RACKET RACKET_HEIGHT
#define RACKET_SPEED 1
#define MAX_RACKET_SPEED 12
#define MIN_RACKET_SPEED 0
#define RACKET_ACC 0.9 // acceleration
#define RACKET_DEC 0.1 // deceleration
#define MID_RACKET_ANGLE_SPEED 6
#define ANGLE_MODULO 360
    
    // Ball constants
#define LITTLE_BALL_DIAMETER 20
#define MAX_X_BALL_SPEED 4.6
#define MAX_Y_BALL_SPEED 4.6

#define INITIAL_SPEED  MAX_X_BALL_SPEED/1;
#define MIN_DEVIATION 0.7
#define MIN_DEVIATION_RANGE 1
#define UNBLOCK_ACTIVATED 3

    // Buttons constants
#define BUTTON_TEXT_ORIGIN 15
#define BUTTON_TEXT_MARGIN 30
#define BUTTON_STATES_NUMBER 3
    
    //Menu textures/images
#define SPACE_BACKGROUND "img/space1.jpg"  
    
    // Units to place drawables in a menu
#define XU WINDOW_WIDTH/8 //x-Unit
#define YU WINDOW_HEIGHT/12 // y-Unit
    
    // Score-related
#define SCORE_INCREMENT 3
#define SCORE_INTERVAL 3000
            
    //Time-related
#define UPDATE_INTERVAL 1000/60
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTES_H */


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

#ifdef	__cplusplus
extern "C" {
#endif

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
#define BRICK_AREA_X_ORIGIN_NUMBER 4 // X component of the area
#define BRICK_AREA_Y_ORIGIN_NUMBER 7 // Y component of the area
#define BRICK_AREA_X_NUMBER 12 // Width of this area, in number of bricks
#define BRICK_AREA_Y_NUMBER 16// Height of the area, in number of bricks
    
    
    // Rackets constants
#define RACKET_WIDTH 80
#define RACKET_HEIGHT 20
#define BASE_RACKET_NUMBER 4
#define TOTAL_RACKET_NUMBER BASE_RACKET_NUMBER + 1
#define MARGIN_BOTTOM_RACKET RACKET_HEIGHT
#define MAX_RACKET_SPEED 12
#define MIN_RACKET_SPEED 0
#define RACKET_ACC 0.8 // acceleration
#define RACKET_DEC 0.1 // deceleration
#define MID_RACKET_ANGLE_SPEED 6
#define ANGLE_MODULO 360
    
    // Ball constants
#define LITTLE_BALL_DIAMETER 20
#define MAX_X_BALL_SPEED 4.6
#define MAX_Y_BALL_SPEED 4.6
#define INITIAL_SPEED  MAX_X_BALL_SPEED/1;
    
    // Buttons
#define BUTTON_TEXT_ORIGIN 15
#define BUTTON_TEXT_MARGIN 30
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTES_H */


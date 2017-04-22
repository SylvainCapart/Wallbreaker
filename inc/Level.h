/* 
 * File:   Level.h
 * Author: Sylvain
 * 
 * This class is here for level management, ie for the moment loading the level
 * files and getting it in a 2D table.
 *
 * Created on 29 janvier 2017, 11:08
 */

#ifndef LEVEL_H
#define	LEVEL_H

#include "Const.h"
#include "Types.h"
#include "Brick.h"
#include <iostream>
#include <fstream>


class Level {
public:
    Level();
    
    int  loadLevel(Brick * brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]);
    void setLevelId(int levelId);
    
private:
    int levelId; // id of the level, used during the choice of the level

};

#endif	/* LEVEL_H */


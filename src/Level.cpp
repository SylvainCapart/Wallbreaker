/* 
 * File:   Level.cpp
 * Author: Sylvain
 * 
 * This class is here for level management, ie for the moment loading the level
 * files and getting it in a 2D table.
 * 
 * Created on 29 janvier 2017, 11:08
 */

#include "Level.h"

using namespace std;

Level::Level(): levelId(0) {

}

void Level::setLevelId(int levelId) {
    this->levelId = levelId;
}

/* Function : loadLevel
 * 
 * Input : int brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER] : the 2D table
 * to fill with 0 1 or 2 following the characters in the loaded file.
 * 
 * Output : 1 on success or 0 on failure
 * 
 * Goal : fill the brickMap with correct data, used by class Game.
 * 
 * TODO v0.2 : improvement
 * Replace the C code with adapted C++ code for file management.
 */

int Level::loadLevel(Brick * brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]) {
    FILE * file = NULL;
    char fileLign[(BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER)] = {0};

    if (levelId == 1)
        file = fopen("lvl/niveau03.lvl", "r");


    if (file == NULL) {
        cout << endl << "ERROR : Level file not found. Regenerate Makelist" << endl;
        return 0;
    }

    fgets(fileLign, BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER + 1, file);

    for (int i = 0; i < BRICK_AREA_X_NUMBER; i++) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; j++) {
            switch (fileLign[(i * BRICK_AREA_Y_NUMBER) + j]) {
                case '0':
                    brickMap[i][j] = new Brick((BRICK_AREA_X_ORIGIN_NUMBER + i) * BRICK_WIDTH, (BRICK_AREA_Y_ORIGIN_NUMBER + j) * BRICK_HEIGHT, 0);
                    break;
                case '1':
                    brickMap[i][j] = new Brick((BRICK_AREA_X_ORIGIN_NUMBER + i) * BRICK_WIDTH, (BRICK_AREA_Y_ORIGIN_NUMBER + j) * BRICK_HEIGHT, 1);
                    break;
                case '2':
                    brickMap[i][j] = new Brick((BRICK_AREA_X_ORIGIN_NUMBER + i) * BRICK_WIDTH, (BRICK_AREA_Y_ORIGIN_NUMBER + j) * BRICK_HEIGHT, 2);
                    break;
                default:
                    break;
            }
        }
    }

    fclose(file);
    return 1;
}



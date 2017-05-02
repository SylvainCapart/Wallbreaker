/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameState.cpp
 * Author: user
 * 
 * Created on 23 avril 2017, 14:50
 */

#include "GameState.h"
#include "Brick.h"
#include "Game.h"
#include <iostream>

using namespace std;

GameState::GameState(SDL_Renderer * renderer, Game & game):m_levelId(1),m_renderer(renderer), m_game(game) {
    m_middleRacketPresent = true;
    m_collisionActivated = true;
    m_middleRacketRotationActivated = true;
    m_levelSelected = false;
    m_won = false;
    m_initialRacketId = 0;
    m_ball = new Ball();
    m_ballTexture = IMG_LoadTexture(renderer, "img/ball2.png");
    if (NULL == m_ballTexture)
        cout << "Error charging ball graphics" << endl;
    
    m_rackets[BOTTOM] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET, HORIZONTAL);
    m_rackets[TOP] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, RACKET_HEIGHT / 2, HORIZONTAL);
    m_rackets[LEFT] = new Racket(RACKET_HEIGHT / 2, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2, VERTICAL);
    m_rackets[RIGHT] = new Racket(WINDOW_WIDTH - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2 - MARGIN_BOTTOM_RACKET, VERTICAL);
    
    
    if (m_middleRacketPresent) {
        m_rackets[MIDDLE] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT / 2 - RACKET_HEIGHT / 2, HORIZONTAL);
    }
    
    m_cobalt = new Color(34, 66, 124);
    m_darkCobalt = new Color(23, 45, 84);
    m_red = new Color(255, 0, 0);
    m_blueGreen = new Color(18, 179, 174);
    m_darkBlueGreen = new Color(9, 45, 45);
    m_yellow = new Color(247, 236, 133);
    m_paleOrange = new Color(255, 178, 102);
    m_green = new Color(0, 204, 0);
    m_black = new Color(0, 0, 0);
    
}

void GameState::init() {
    m_levelSelected = false;
    m_middleRacketAngle = 0;
    m_won = false;
    
    m_ball->setLaunched(false);
    placeBall();
    
    
    if (!m_levelSelected) {
        loadLevel(m_brickMap);
        m_levelSelected = true;
    }
}

GameState::~GameState() {
    SDL_DestroyTexture(m_ballTexture);
    for (int i = 0; i < TOTAL_RACKET_NUMBER; ++i) {
        delete m_rackets[i];
        m_rackets[i] = NULL;
    }
    delete m_ball;
    m_ball = NULL;
    
    delete m_green;
    delete m_cobalt;
    delete m_darkCobalt;
    delete m_red;
    delete m_black;
    delete m_blueGreen;
    delete m_darkBlueGreen;
    delete m_yellow;
    delete m_paleOrange;
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
 * TODO v0.3 : improvement
 * Replace the C code with adapted C++ code for file management.
 */

int GameState::loadLevel(Brick * brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER]) {
    FILE * file = NULL;
    char fileLign[(BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER)] = {0};
    
    if (1 == m_levelId)
        file = fopen("lvl/niveau03.lvl", "r");
    
    
    if (NULL == file) {
        cout << endl << "ERROR : Level file not found. Regenerate Makelist" << endl;
        return 0;
    }
    
    fgets(fileLign, BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER + 1, file);
    
    for (int i = 0; i < BRICK_AREA_X_NUMBER; ++i) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; ++j) {
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

/* Function : draw
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : included in the main thread, the draw() function handles :
 *        - ball, rackets, brick drawings with the SDL_Renderer
 *        - ball vs bricks collisions, updating brickMap after collision
 *        - ball vs rackets collisions : Collision class detects collision and draw()
 * method set the new speed following different parameters 
 *
 * TODO : optimisation
 * Simplify the function, creates sub-functions, separate the different activities
 * 
 * DONE 0.1 Avoid to draw bricks at each draw(), but redraw once a collision has been detected
 * 
 */

void GameState::draw(){
    
    SDL_Rect racketRect; // drawing area for a racket
    bool contact = false; // contact ball/racket
    bool brickContact = false; // contact ball/brick
    float tempRatio; // temp variable for speed modifications
    int coeffSpeedModif; // variable for speed modifications
    int racketsNumber; // number of rackets
    Vector rotatedBallSpeed; // speed of the ghost ball used for the collision on
    //the rotated racket
    float squareNormSpeed; // norm speed
    SDL_Rect brickRect;
    brickRect.w = BRICK_WIDTH;
    brickRect.h = BRICK_HEIGHT;
    Vector normal, newSpeed, oldSpeed; // vectors used for speed changes
    Point A, B, C; // points used for collision
    SegmentPoints AB; // rebound segment of an obstacle
    SDL_Rect ballRect; // rectangle/square surrounding the ball
    Circle ballCircle; // ball described by its center coordinates and radius
    Color * screenColor = m_cobalt;
    
    if (!m_ball->isLaunched()) {
        switch (m_initialRacketId) {
            case BOTTOM:
                m_ball->setX(m_rackets[BOTTOM]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2);
                break;
            case RIGHT:
                m_ball->setY(m_rackets[RIGHT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2);
                break;
            case LEFT:
                m_ball->setY(m_rackets[LEFT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2);
                break;
            case TOP:
                m_ball->setX(m_rackets[TOP]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2);
                break;
        }
    } else
        m_ball->move();
    
    ballRect.x = m_ball->getX();
    ballRect.y = m_ball->getY();
    ballRect.w = m_ball->getW();
    ballRect.h = m_ball->getH();
    
    ballCircle.radius = LITTLE_BALL_DIAMETER / 2;
    ballCircle.x = m_ball->getX() + ballCircle.radius;
    ballCircle.y = m_ball->getY() + ballCircle.radius;
    
    oldSpeed.x = m_ball->getX_speed(); // x-speed before impact
    oldSpeed.y = m_ball->getY_speed(); // y-speed before impact
    
    squareNormSpeed = oldSpeed.x * oldSpeed.x + oldSpeed.y * oldSpeed.y; // norm
    
    // place the ball
    if (!m_ball->isPlaced() && !m_ball->isLaunched()) {
        placeBall();
    }
    
    // setting the screen color
    SDL_SetRenderDrawColor(m_renderer, screenColor->r, screenColor->g, screenColor->b, SDL_ALPHA_OPAQUE); // setting background color
    SDL_RenderClear(m_renderer); // clearing present renderer
    
    // check collision of the ball with bricks
    for (int i = 0; i < BRICK_AREA_X_NUMBER && !brickContact; ++i) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER && !brickContact; ++j) {
            switch (m_brickMap[i][j]->GetResistance()) {
                case 0:
                    break;
                    
                case 1:
                case 2:
                    
                    brickRect.x = m_brickMap[i][j]->GetX();
                    brickRect.y = m_brickMap[i][j]->GetY();
                    brickContact = Collision::collisionCircleAABB(ballCircle, brickRect); // true if collision
                    
                    if (true == brickContact) {
                        AB = Collision::rebound(brickRect, ballRect, oldSpeed);
                        
                        A.x = AB.A.x; // segment AB where the ball will do the rebound
                        A.y = AB.A.y;
                        B.x = AB.B.x;
                        B.y = AB.B.y;
                        C.x = m_ball->getXCenter();
                        C.y = m_ball->getYCenter();
                        
                        normal = Collision::getNormal(A, B, C); // we will the the projection of C on the AB segment, and then the normal
                        // vector to the segment on this projection
                        
                        newSpeed = Collision::calculateVectorV2(oldSpeed, normal); // getting new speed following the incoming vector
                        // speed, regarding the normal vector
                        
                        m_ball->setX_speed(newSpeed.x);
                        m_ball->setY_speed(newSpeed.y); //
                        --(*m_brickMap[i][j]); // decreasing resistance 1->0 or 2->1 after collision 
                    }
                    break;
                    
            }
            
        }
    }
    
    // ****** Render bricks ******
    
    // Render the bricks after collision is made
    buildBrickMap();
    
    SDL_RenderCopy(m_renderer, m_ballTexture, NULL, &ballRect); // copy the rectangle on the renderer
    
    // ****** Render the rackets ******
    if (m_middleRacketPresent) // depends if we want a middle racket or not
        racketsNumber = TOTAL_RACKET_NUMBER;
    else
        racketsNumber = BASE_RACKET_NUMBER;
    
    Point rotOrigin; // position of middle racket center
    rotOrigin.x = m_rackets[MIDDLE]->getRacketCenter().x;
    rotOrigin.y = m_rackets[MIDDLE]->getRacketCenter().y;
    
    for (int i = 0; i < racketsNumber; ++i) {
        
        if (m_middleRacketPresent && (MIDDLE == i)) {
            
            racketRect.x = m_rackets[MIDDLE]->getX();
            racketRect.y = m_rackets[MIDDLE]->getY();
            racketRect.w = m_rackets[MIDDLE]->getW();
            racketRect.h = m_rackets[MIDDLE]->getH();
            
            SDL_Texture * myText = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RACKET_WIDTH, RACKET_HEIGHT);
            
            SDL_SetRenderTarget(m_renderer, myText);
            SDL_SetRenderDrawColor(m_renderer, m_blueGreen->r, m_blueGreen->g, m_blueGreen->b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(m_renderer, NULL);
            SDL_SetRenderTarget(m_renderer, NULL);
            
            SDL_QueryTexture(myText, NULL, NULL, &racketRect.w, &racketRect.h);
            
            //render a rectangle rotated by middleRacketAngle
            SDL_RenderCopyEx(m_renderer, myText, NULL, &racketRect, m_middleRacketAngle, NULL, SDL_FLIP_NONE);
            
        } else { // top bottom right and left rendering
            racketRect.x = m_rackets[i]->getX();
            racketRect.y = m_rackets[i]->getY();
            racketRect.w = m_rackets[i]->getW();
            racketRect.h = m_rackets[i]->getH();
            setRenderDrawColor(m_renderer, m_darkBlueGreen);
            SDL_RenderDrawLine(m_renderer, racketRect.x, racketRect.y, racketRect.x + racketRect.w, racketRect.y);
            SDL_RenderDrawLine(m_renderer, racketRect.x + racketRect.w, racketRect.y, racketRect.x + racketRect.w, racketRect.y + racketRect.h);
            SDL_RenderDrawLine(m_renderer, racketRect.x + racketRect.w, racketRect.y + racketRect.h, racketRect.x, racketRect.y + racketRect.h);
            SDL_RenderDrawLine(m_renderer, racketRect.x, racketRect.y + racketRect.h, racketRect.x, racketRect.y);
            fillRectGame(&racketRect, *m_blueGreen); // sets the color + draw the rectangle with it
        }
        
        
        
        // ****** Handle racket collision ****** 
        if (i != MIDDLE)
            contact = Collision::collisionCircleAABB(ballCircle, racketRect); // detects collision with the racket
        else {
            Circle rotatedBallCircle; // ghost ball for collision on a non rotated middle racket
            
            rotatedBallCircle.radius = LITTLE_BALL_DIAMETER / 2;
            
            // ghost ball (rotated one) is the normal ball rotated from minus angle the racket has been rotated
            rotatedBallCircle.x = rotOrigin.x + (ballCircle.x - rotOrigin.x) * cos(3.14 * m_middleRacketAngle / 180)
                    + (ballCircle.y - rotOrigin.y) * sin(3.14 * m_middleRacketAngle / 180);
            rotatedBallCircle.y = rotOrigin.y - (ballCircle.x - rotOrigin.x) * sin(3.14 * m_middleRacketAngle / 180)
                    + (ballCircle.y - rotOrigin.y) * cos(3.14 * m_middleRacketAngle / 180);
            
            // the ghost ball has also its speed vector rotated
            rotatedBallSpeed.x = (m_ball->getX_speed()) * cos(3.14 * m_middleRacketAngle / 180)
                    + (m_ball->getY_speed()) * sin(3.14 * m_middleRacketAngle / 180);
            rotatedBallSpeed.y = -(m_ball->getX_speed()) * sin(3.14 * m_middleRacketAngle / 180)
                    + (m_ball->getY_speed()) * cos(3.14 * m_middleRacketAngle / 180);
            
            // definition of the ghost Circle ball
            SDL_Rect rotatedBallRect = {rotatedBallCircle.x - rotatedBallCircle.radius,
            rotatedBallCircle.y - rotatedBallCircle.radius,
            rotatedBallCircle.radius * 2,
            rotatedBallCircle.radius * 2};
            
            //To display the ghost ball :
            //SDL_RenderDrawLine(renderer, rotatedBallCircle.x, rotatedBallCircle.y, rotatedBallCircle.x + 10 * rotatedBallSpeed.x, rotatedBallCircle.y + 10 * rotatedBallSpeed.y);
            //SDL_RenderCopy(renderer, this->ballTexture, NULL, &rotatedBallRect);
            
            Point closest; // to detect the closest point with the ghost ball on the non rotated racket
            
            // Find the unrotated closest x point from center of rotated circle
            if (rotatedBallCircle.x < racketRect.x)
                closest.x = racketRect.x;
            else if (rotatedBallCircle.x > racketRect.x + racketRect.w)
                closest.x = racketRect.x + racketRect.w;
            else
                closest.x = rotatedBallCircle.x;
            
            // Find the unrotated closest y point from center of unrotated circle
            if (rotatedBallCircle.y < racketRect.y)
                closest.y = racketRect.y;
            else if (rotatedBallCircle.y > racketRect.y + racketRect.h)
                closest.y = racketRect.y + racketRect.h;
            else
                closest.y = rotatedBallCircle.y;
            
            
            // Determine collision
            float squareDistance = (rotatedBallCircle.x - closest.x) * (rotatedBallCircle.x - closest.x)
            + (rotatedBallCircle.y - closest.y) * (rotatedBallCircle.y - closest.y);
            
            // if distance between the ghost ball and the closest point on the non rotated racket
            // is lower than the ball radius
            if (squareDistance <= rotatedBallCircle.radius * rotatedBallCircle.radius) {
                contact = true; // Collision !
                m_middleRacketRotationActivated = false; // rotation is deactivated during a brief moment
            } else
                contact = false; // othewise, no collision
            
            if (squareDistance >= rotatedBallCircle.radius * rotatedBallCircle.radius + 2000) {
                // if the distance is greater than radius + an arbitrary constant
                m_middleRacketRotationActivated = true; // we reactivate rotation
                m_collisionActivated = true; // we reactivate collision
            }
            
            if (contact && m_collisionActivated) { // if we detect a collision and collision is activated
                
                Vector newSpeedBeforeReturnRotation;
                
                // selection of the rebound segment
                AB = Collision::rebound(racketRect, rotatedBallRect, rotatedBallSpeed);
                
                A.x = AB.A.x;
                A.y = AB.A.y;
                B.x = AB.B.x;
                B.y = AB.B.y;
                C.x = rotatedBallCircle.x;
                C.y = rotatedBallCircle.y;
                
                normal = Collision::getNormal(A, B, C); // normal to the rebound segment
                
                // new speed of the ghost ball
                newSpeedBeforeReturnRotation = Collision::calculateVectorV2(rotatedBallSpeed, normal);
                
                // we rotate the speed in the other direction to set the normal ball speed
                newSpeed.x = newSpeedBeforeReturnRotation.x * cos(3.14 * m_middleRacketAngle / 180)
                        - (newSpeedBeforeReturnRotation.y) * sin(3.14 * m_middleRacketAngle / 180);
                newSpeed.y = (newSpeedBeforeReturnRotation.x) * sin(3.14 * m_middleRacketAngle / 180)
                        + (newSpeedBeforeReturnRotation.y) * cos(3.14 * m_middleRacketAngle / 180);
            }
        }
        
        // collision with the other rackets 
        if (contact && m_ball->isLaunched()) {
            switch (i) {
                case BOTTOM:
                    A.x = m_rackets[i]->getX(); // AB segment where rebound is done 
                    A.y = m_rackets[i]->getY();
                    B.x = A.x + m_rackets[i]->getW();
                    B.y = A.y;
                    C.x = ballCircle.x; // C center of the ball during impact
                    C.y = ballCircle.y;
                    
                    normal = Collision::getNormal(A, B, C);
                    newSpeed = Collision::calculateVectorV2(oldSpeed, normal);
                    
                    tempRatio = getRacketContactRatio(A, B, C);
                    
                    // empiric and arbitrary speed modification following racket's speed
                    coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + m_rackets[i]->getRacketSpeed();
                    
                    // setting the new speed. 0.5 because we count from the middle of the racket.
                    // if tempratio = 0.5 ( ie hit at the center of the racket ) then 
                    // the new speed is not modified and is like a "normal" rebound
                    newSpeed.x = newSpeed.x - coeffSpeedModif * (0.5 - tempRatio);
                    
                    // the other speed component is defined regarding the first one,
                    // so that speed norm remains unchanged.
                    newSpeed.y = -sqrt(abs(squareNormSpeed - newSpeed.x * newSpeed.x));
                    break;
                case TOP:
                    // see above the BOTTOM case
                    A.x = m_rackets[i]->getX();
                    A.y = m_rackets[i]->getY() + m_rackets[i]->getH();
                    B.x = A.x + m_rackets[i]->getW();
                    B.y = A.y;
                    C.x = ballCircle.x;
                    C.y = ballCircle.y;
                    
                    normal = Collision::getNormal(A, B, C);
                    newSpeed = Collision::calculateVectorV2(oldSpeed, normal);
                    
                    tempRatio = getRacketContactRatio(A, B, C);
                    
                    coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + m_rackets[i]->getRacketSpeed();
                    newSpeed.x = newSpeed.x - coeffSpeedModif * (0.5 - tempRatio);
                    newSpeed.y = sqrt(abs(squareNormSpeed - newSpeed.x * newSpeed.x));
                    break;
                case LEFT:
                    // see above the BOTTOM case
                    A.x = m_rackets[i]->getX() + m_rackets[i]->getW();
                    A.y = m_rackets[i]->getY();
                    B.x = A.x;
                    B.y = A.y + m_rackets[i]->getH();
                    C.x = ballCircle.x;
                    C.y = ballCircle.y;
                    
                    normal = Collision::getNormal(A, B, C);
                    newSpeed = Collision::calculateVectorV2(oldSpeed, normal);
                    
                    tempRatio = getRacketContactRatio(A, B, C);
                    
                    coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + m_rackets[i]->getRacketSpeed();
                    newSpeed.y = newSpeed.y - coeffSpeedModif * (0.5 - tempRatio);
                    newSpeed.x = sqrt(abs(squareNormSpeed - newSpeed.y * newSpeed.y));
                    break;
                case RIGHT:
                    // see above the BOTTOM case
                    A.x = m_rackets[i]->getX();
                    A.y = m_rackets[i]->getY();
                    B.x = A.x;
                    B.y = A.y + m_rackets[i]->getH();
                    C.x = ballCircle.x;
                    C.y = ballCircle.y;
                    normal = Collision::getNormal(A, B, C);
                    newSpeed = Collision::calculateVectorV2(oldSpeed, normal);
                    
                    tempRatio = getRacketContactRatio(A, B, C);
                    
                    coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + m_rackets[i]->getRacketSpeed();
                    newSpeed.y = newSpeed.y - coeffSpeedModif * (0.5 - tempRatio);
                    newSpeed.x = -sqrt(abs(squareNormSpeed - newSpeed.y * newSpeed.y));
                    break;
                case MIDDLE:
                    break;
                default:
                    break;
            }
            if (m_collisionActivated) {
                
                m_ball->setX_speed(newSpeed.x);
                m_ball->setY_speed(newSpeed.y);
                
                m_ball->setX(m_ball->getX() + newSpeed.x);
                m_ball->setY(m_ball->getY() + newSpeed.y);
            }
        }
    }
    
    SDL_RenderPresent(m_renderer); // displays the renderer where we put all the graphics before
    checkBrickPresence();
}


/* Function : buildBrickMap
 * 
 * Input : None
 * 
 * Output : None
 * 
 * Goal : displays the brickMap at the screen
 */

void GameState::buildBrickMap() {
    
    SDL_Rect brickRect;
    brickRect.w = BRICK_WIDTH;
    brickRect.h = BRICK_HEIGHT;
    
    for (int i = 0; i < BRICK_AREA_X_NUMBER; ++i) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; ++j) {
            switch (m_brickMap[i][j]->GetResistance()) {
                case 0:
                    break;
                    
                case 1:
                    brickRect.x = m_brickMap[i][j]->GetX();
                    brickRect.y = m_brickMap[i][j]->GetY();
                    
                    fillRectGame(&brickRect, *m_yellow); // sets the color + draw the rectangle with it
                    
                    setRenderDrawColor(m_renderer, m_darkCobalt);
                    SDL_RenderDrawLine(m_renderer, brickRect.x, brickRect.y, brickRect.x + brickRect.w, brickRect.y);
                    SDL_RenderDrawLine(m_renderer, brickRect.x + brickRect.w, brickRect.y, brickRect.x + brickRect.w, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(m_renderer, brickRect.x + brickRect.w, brickRect.y + brickRect.h, brickRect.x, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(m_renderer, brickRect.x, brickRect.y + brickRect.h, brickRect.x, brickRect.y);
                    
                    break;
                    
                case 2:
                    brickRect.x = m_brickMap[i][j]->GetX();
                    brickRect.y = m_brickMap[i][j]->GetY();
                    
                    fillRectGame(&brickRect, *m_paleOrange);
                    
                    setRenderDrawColor(m_renderer, m_darkCobalt);
                    SDL_RenderDrawLine(m_renderer, brickRect.x, brickRect.y, brickRect.x + brickRect.w, brickRect.y);
                    SDL_RenderDrawLine(m_renderer, brickRect.x + brickRect.w, brickRect.y, brickRect.x + brickRect.w, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(m_renderer, brickRect.x + brickRect.w, brickRect.y + brickRect.h, brickRect.x, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(m_renderer, brickRect.x, brickRect.y + brickRect.h, brickRect.x, brickRect.y);
                    break;
            }
        }
    }
}

/* Function : getRacketContactRatio
 * 
 * Input : const Point & A, const Point & B,const  Point & C, axis direction : AB
 * surface where the ball hits, C center of the ball when impact occurs
 * 
 * Output : a float ratio between 0.0 and 1.0
 * 
 * Goal : to know in what part of the racket hits the racket and apply a directional
 * modification to the new speed.
 */

float GameState::getRacketContactRatio(const Point & A, const Point & B, const Point & C) {
    float ratio;
    ratio = Collision::getDistanceBetweenTwoPoints(A, C) / Collision::getDistanceBetweenTwoPoints(A, B);
    
    if (ratio > 1)
        ratio = 1;
    if (ratio < 0)
        ratio = 0;
    
    return ratio;
    
}

/* Function : getSpeedModifCoeffFromContactRatio
 * 
 * Input : float contactRatio
 * Output : an int coeff
 * Goal : return an arbitrary coefficient following input contact ratio, for speed
 * modification
 */

int GameState::getSpeedModifCoeffFromContactRatio(float contactRatio) {
    
    int coeff = 2;
    
    if ((contactRatio >= 0.35) || (contactRatio <= 0.65))
        coeff = 2;
    else if ((0.2 < contactRatio && contactRatio < 0.35) || (0.65 < contactRatio && contactRatio < 0.8))
        coeff = 4;
    else if ((contactRatio < 0.2) || (contactRatio > 0.8))
        coeff = 10;
    
    
    return coeff;
}

/* Function : checkBrickPresence
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : if an int in brickMap is not equal to 0, change the game state to WON and
 * erase the game mode graphical interface.
 */

void GameState::checkBrickPresence() {
    
    m_won = true;
    for (int i = 0; i < BRICK_AREA_X_NUMBER; ++i) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; ++j) {
            switch (m_brickMap[i][j]->GetResistance()) {
                case 0:
                    break;
                case 1:
                    m_won = false;
                    break;
                case 2:
                    m_won = false;
                    break;
            }
        }
    }
    if (true == m_won) {

        m_won = false;
        m_game.setCurrentState(SCORE_STATE);
        
    }
    
}

/* Function : placeBall
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : place the ball on a random racket
 */

void GameState::placeBall() {
    
    m_initialRacketId = rand() % 4;
    
    m_ball->setInitialRacketId(m_initialRacketId);
    
    switch (m_initialRacketId) {
        case BOTTOM:
            m_ball->setX(m_rackets[BOTTOM]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            m_ball->setY(m_rackets[BOTTOM]->getY() - LITTLE_BALL_DIAMETER);
            break;
        case TOP:
            m_ball->setX(m_rackets[TOP]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            m_ball->setY(m_rackets[TOP]->getY() + RACKET_HEIGHT);
            break;
        case LEFT:
            m_ball->setX(m_rackets[LEFT]->getX() + RACKET_HEIGHT);
            m_ball->setY(m_rackets[LEFT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2 - 0.1);
            
            break;
        case RIGHT:
            m_ball->setX(m_rackets[RIGHT]->getX() - LITTLE_BALL_DIAMETER);
            m_ball->setY(m_rackets[RIGHT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            break;
    }
    
    m_ball->setPlaced(true);
}


void GameState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;
}

void GameState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    getProperRacket(evt->key.keysym.sym)->decelerate();
    if (SDLK_ESCAPE == evt->key.keysym.sym) {
        
        m_levelSelected = false;
        m_ball->setLaunched(false);
        m_ball->setPlaced(false);
        m_middleRacketAngle = 0;
        
        m_game.setCurrentState(MENU_STATE);
    }
}

void GameState::fillRectGame(SDL_Rect* rc, int r, int g, int b) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, rc);
}

void GameState::fillRectGame(SDL_Rect* rc, const Color& color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, rc);
}

void GameState::onQuit() {
    m_game.stop();
}

void GameState::update() {
    if (m_keys[SDLK_LEFT]) {
        m_rackets[BOTTOM]->moveLeft();
    }
    if (m_keys[SDLK_RIGHT]) {
        m_rackets[BOTTOM]->moveRight();
    }
    if (m_keys[SDLK_UP]) {
        m_rackets[RIGHT]->moveUp();
    }
    if (m_keys[SDLK_DOWN]) {
        m_rackets[RIGHT]->moveDown();
    }
    if (m_keys[SDLK_z]) {
        m_rackets[LEFT]->moveUp();
    }
    if (m_keys[SDLK_q]) {
        m_rackets[TOP]->moveLeft();
    }
    if (m_keys[SDLK_s]) {
        m_rackets[LEFT]->moveDown();
    }
    if (m_keys[SDLK_d]) {
        m_rackets[TOP]->moveRight();
    }
    if (m_keys[SDLK_SPACE]) {
        if (m_ball->isPlaced() && !m_ball->isLaunched()) {
            m_ball->setInitialSpeed(m_initialRacketId);
            m_ball->launch();
        }
        if (m_middleRacketRotationActivated) {
            
            m_middleRacketAngle = (m_middleRacketAngle + MID_RACKET_ANGLE_SPEED) % ANGLE_MODULO;
            
        }
    }
    
}

void GameState::setRenderDrawColor(SDL_Renderer * renderer, Color* color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, SDL_ALPHA_OPAQUE);
}

void GameState::onEvent(SDL_Event* evt) {
    
    switch (evt->type) {
        case SDL_QUIT: onQuit();
        break;
        case SDL_KEYDOWN: onKeyDown(evt);
        break;
        case SDL_KEYUP: onKeyUp(evt);
        break;
        
    }
}
Racket * GameState::getProperRacket(SDL_Keycode keyId) {
    switch(keyId) {
        case SDLK_LEFT:
        case SDLK_RIGHT:
            return m_rackets[BOTTOM];
            break;
        case SDLK_UP:
        case SDLK_DOWN:
            return m_rackets[RIGHT];
            break;
        case SDLK_z:
        case SDLK_s:
            return m_rackets[LEFT];
            break;
        case SDLK_q:
        case SDLK_d:
            return m_rackets[TOP];
            break;
        default : 
            return m_rackets[BOTTOM];
    }
}





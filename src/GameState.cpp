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

#include "Collision.h"
#include "MenuState.h"
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <cmath>

using namespace std;

GameState::GameState(SDL_Renderer * renderer, Game & game, StateContext & stateContext): m_renderer(renderer),
        m_game(game), m_stateContext(stateContext), m_score(0), m_past(0), m_pastScore(0), m_blockCounter(0) {
    m_menu = new Menu(m_renderer);
    m_middleRacketPresent = true;
    m_levelSelected = false;
    m_won = false;
    m_initialRacketId = BOTTOM;
    
    
    m_screenColor = new Color(Colors::st_cobalt.r,Colors::st_cobalt.g,Colors::st_cobalt.b);
    
    m_rackets[BOTTOM] = m_menu->createRacket( WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET,
            HORIZONTAL, Colors::st_blueGreen, Colors::st_darkCobalt, K_LEFT, K_RIGHT, BOTTOM);
    m_rackets[TOP] = m_menu->createRacket( WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, RACKET_HEIGHT / 2,
            HORIZONTAL, Colors::st_blueGreen, Colors::st_darkCobalt, K_Q, K_D, TOP);
    m_rackets[LEFT] = m_menu->createRacket( RACKET_HEIGHT / 2, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2,
            VERTICAL, Colors::st_blueGreen, Colors::st_darkCobalt, K_S, K_Z, LEFT);
    m_rackets[RIGHT] = m_menu->createRacket( WINDOW_WIDTH - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2 - MARGIN_BOTTOM_RACKET,
            VERTICAL, Colors::st_blueGreen, Colors::st_darkCobalt, K_UP, K_DOWN, RIGHT);
    
    if (m_middleRacketPresent) {
        m_rackets[MIDDLE] = m_menu->createRotatingRacket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT / 2 - RACKET_HEIGHT / 2,
                Colors::st_blueGreen, Colors::st_darkCobalt, K_SPACE, K_NULL, MIDDLE);
    }
    
    m_ball = m_menu->createBall("img/ball2.png");
    
    m_menu->createText(Fonts::st_infoFont, "Score :", Colors::st_white, 6.3, 0.5, 1, 0.7);
    m_scoreText = m_menu->createText(Fonts::st_numberFont, to_string(m_score).c_str(), Colors::st_white, 7.4, 0.5, 0.3, 0.7);
    
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            m_brickMap[i][j] = new Brick(i * BRICK_WIDTH, j * BRICK_HEIGHT , 0, m_renderer);
        }
    }
    
}

void GameState::init() {
    
    string demoFileName = "lvl/niveau00.lvl";
    string editedFileName = "lvl/editedLevel.lvl";
    m_levelSelected = false;
    m_won = false;
    m_score = 0;
    m_scoreText->changeText("0");
    m_pastScore = 0;
    m_blockCounter = 0;
    
    m_ball->setLaunched(false);
    m_initialRacketId = m_ball->placeBall(m_rackets);
    
    if (!m_levelSelected) {
        if (RANDOM_LEVEL == m_stateContext.getLoadLevelType())
            loadRandomLevel(m_brickMap);
        else if (DEMO_LEVEL == m_stateContext.getLoadLevelType())
            loadFileLevel(m_brickMap, demoFileName);
        else // EDITED_LEVEL
            loadFileLevel(m_brickMap, editedFileName);
        
        m_levelSelected = true;
    }
}

GameState::~GameState() {
    
    delete m_screenColor;
    delete m_menu;
    
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            delete m_brickMap[i][j];
        }
    }
    
}

std::string GameState::to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
/* Function : loadFileLevel
 * 
 * Input : int brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER] : the 2D table
 * to fill with numbers following the characters in the loaded file.
 * 
 * Output : true on success or 0false on failure
 * 
 * Goal : fill the brickMap with correct data, used by class Game.
 * 
 * TODO v0.3 : improvement
 * Replace the C code with adapted C++ code for file management.
 */

bool GameState::loadFileLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string& fileName) {
    string fileLign;
    stringstream buffer;
    ifstream file(fileName.c_str());
    if (file.good())
    {
        buffer << file.rdbuf();
    }
    else
    {
        cerr << "ERROR : Level file not found. Try regenerating Makelist" << endl;
        return false;
    }
    fileLign = buffer.str();
    
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            brickMap[i][j]->SetResistance((fileLign[i  * NB_BRICKS_WIDTH + j] - '0') % 48);
            if (m_middleRacketPresent && getIntSqrDistance(m_rackets[MIDDLE]->getX() +
                    m_rackets[MIDDLE]->getW()/2, m_rackets[MIDDLE]->getY() + + m_rackets[MIDDLE]->getH()/2,
                    brickMap[i][j]->GetX() + brickMap[i][j]->GetW()/2, brickMap[i][j]->GetY() + brickMap[i][j]->GetH()/2  ) 
                    < m_rackets[MIDDLE]->getW() * m_rackets[MIDDLE]->getW() / 4)
                brickMap[i][j]->SetResistance(0); //empty the bricks in the middle if there is a middle racket
        }
    }
    file.close();
    return true;
}

/* Function : loadRandomLevel
 * 
 * Input : int brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER] : the 2D table
 * to fill with numbers following a statistical law
 * 
 * Output : none
 * 
 * Goal : fill the brickMap with correct data, used by class Game.
 */

void GameState::loadRandomLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT]) {
    
    char fileLign[(BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER)] = {0};
    int randNb;
    int brickResistance;
    
    for (int i = 0; i < BRICK_AREA_X_NUMBER * BRICK_AREA_Y_NUMBER; i++)
    {
        randNb = rand() % 20;
        if (randNb >= 0 && randNb <=9)
            brickResistance = 0;
        else if (randNb >= 10 && randNb <= 14)
            brickResistance = 1;
        else if (randNb >= 15 && randNb <= 17)
            brickResistance = 2;
        else if (randNb == 18 )
            brickResistance = 3;
        else
            brickResistance = 4;
        
        
        std::string str = to_string(brickResistance);
        fileLign[i] = str[0];
        
    }
    
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            brickMap[i][j]->SetResistance( (fileLign[((i - BRICK_AREA_X_ORIGIN_NUMBER) * BRICK_AREA_Y_NUMBER) + (j - BRICK_AREA_Y_ORIGIN_NUMBER)] - '0') % 48);
            if (m_middleRacketPresent && getIntSqrDistance(m_rackets[MIDDLE]->getX() +
                    m_rackets[MIDDLE]->getW()/2, m_rackets[MIDDLE]->getY() + + m_rackets[MIDDLE]->getH()/2,
                    brickMap[i][j]->GetX() + brickMap[i][j]->GetW()/2, brickMap[i][j]->GetY() + brickMap[i][j]->GetH()/2  ) 
                    < m_rackets[MIDDLE]->getW() * m_rackets[MIDDLE]->getW() / 4)
                brickMap[i][j]->SetResistance(0); //empty the bricks in the middle if there is a middle racket
            
        }
    }
    
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
 * DONE : optimisation
 * Simplify the function, creates sub-functions, separate the different activities
 * 
 * DONE 0.1 Avoid to draw bricks at each draw(), but redraw once a collision has been detected
 * 
 */

void GameState::draw(){
    bool contact = false; // contact ball/racket
    bool brickContact = false; // contact ball/brick
    float tempRatio; // temp variable for speed modifications
    float squareNormSpeed; // norm speed
    float squareDistance;
    T_RACKET_POSITION closestRacketIndex;
    int8_t coeffSpeedModif; // variable for speed modifications
    int32_t now = SDL_GetTicks();
    int32_t timeElapsed = now - m_past;
    Vector rotatedBallSpeed; // speed of the ghost ball used for the collision on the rotated racket
    Vector normal, newSpeed, oldSpeed; // vectors used for speed changes
    Vector newSpeedBeforeReturnRotation; // vector use for the middle racket rotation
    Point A, B, C; // points used for collision
    Point closest; // to detect the closest point with the ghost ball on the non rotated racket
    Point rotOrigin; // position of middle racket center
    rotOrigin.x = m_rackets[MIDDLE]->getRacketCenter().x;
    rotOrigin.y = m_rackets[MIDDLE]->getRacketCenter().y;
    SegmentPoints AB; // rebound segment of an obstacle
    SDL_Rect ballRect; // rectangle/square surrounding the ball
    SDL_Rect racketRect; // drawing area for a racket
    SDL_Rect brickRect;
    SDL_Rect rotatedBallRect;
    brickRect.w = BRICK_WIDTH;
    brickRect.h = BRICK_HEIGHT;
    
    Circle ballCircle; // ball described by its center coordinates and radius
    Circle rotatedBallCircle; // ghost ball for collision on a non rotated middle racket
    //decrement score every SCORE_INTERVAL
    
    if (timeElapsed > SCORE_INTERVAL)
    {
        if (m_score > 0)
            --m_score;
        m_past = now;
    }
    
    // place the ball
    if (!m_ball->isPlaced() && !m_ball->isLaunched()) {
        m_initialRacketId = m_ball->placeBall(m_rackets);
        m_blockCounter = 0;
    }
    updateRacketsPositions();
    m_ball->update(m_initialRacketId, m_rackets);
    
    
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
    // setting the screen color
    setRenderDrawColor(m_renderer, m_screenColor);
    SDL_RenderClear(m_renderer); // clearing present renderer
    
    // HANDLING BALL/BRICKS COLLISION
    int xIndex = (m_ball->getX() / BRICK_WIDTH);
    int yIndex = (m_ball->getY() / BRICK_HEIGHT);
    
    if (m_ball->isLaunched())
    {
        for (int i = (xIndex - 1); i <= (xIndex + 1)  && !brickContact; ++i) {
            for (int j = (yIndex - 1); j <= (yIndex + 1) && !brickContact; ++j) {
                
                if ((i >= BRICK_AREA_X_ORIGIN_NUMBER) && (i <= BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER)
                        && (j >= BRICK_AREA_Y_ORIGIN_NUMBER) && (j <= BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER))
                {     
                    switch (m_brickMap[i][j]->GetResistance()) {
                        
                        case 0:
                            break;
                            
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            
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
                                
                                if ((newSpeed.x >= - MIN_DEVIATION_RANGE && newSpeed.x <= 0) || (newSpeed.x <= MIN_DEVIATION_RANGE && newSpeed.x >= 0) )
                                    ++m_blockCounter;
                                else if ((newSpeed.y >= - MIN_DEVIATION_RANGE && newSpeed.y <= 0) || (newSpeed.y <= MIN_DEVIATION_RANGE && newSpeed.y >= 0) )
                                    ++m_blockCounter;
                                else
                                    m_blockCounter = 0;
                                if (m_blockCounter >= UNBLOCK_ACTIVATED )
                                {
                                    if ( newSpeed.x <= 0 )
                                        newSpeed.x = newSpeed.x - MIN_DEVIATION;
                                    else
                                        newSpeed.x = newSpeed.x + MIN_DEVIATION;
                                    if ( newSpeed.y <= 0 )
                                        newSpeed.y = newSpeed.y - MIN_DEVIATION;
                                    else 
                                        newSpeed.y = newSpeed.y + MIN_DEVIATION;
                                    m_blockCounter = 0;
                                    cout << "Counter ACTIVATED xv : " << newSpeed.x << " / yv : " << newSpeed.y << endl;
                                }
                                if (isnan(newSpeed.x)) // should not occur but some collision bugs persist
                                    newSpeed.x = -oldSpeed.x;
                                if (isnan(newSpeed.y))
                                    newSpeed.y = -oldSpeed.y;
                                m_ball->setX_speed(newSpeed.x);
                                m_ball->setY_speed(newSpeed.y);
                                m_ball->setX(m_ball->getX() + newSpeed.x);
                                m_ball->setY(m_ball->getY() + newSpeed.y);
                                if (m_brickMap[i][j]->GetResistance() != 4)
                                {
                                    --(*m_brickMap[i][j]); // decreasing resistance after collision 
                                    m_score = m_score + SCORE_INCREMENT;
                                }
                                checkBrickPresence();
                            }
                            break;
                    }
                }
            }
        }
    }
    
    
    // HANDLING BALL_RACKETS COLLISIONS
    if (m_ball->isLaunched() && !brickContact)
    {
        closestRacketIndex = getClosestRacket();
        
        racketRect.x = m_rackets[closestRacketIndex]->getX();
        racketRect.y = m_rackets[closestRacketIndex]->getY();
        racketRect.w = m_rackets[closestRacketIndex]->getW();
        racketRect.h = m_rackets[closestRacketIndex]->getH();
        
        if (MIDDLE != closestRacketIndex)
            contact = Collision::collisionCircleAABB(ballCircle, racketRect);
        else
        {
            rotatedBallCircle.radius = LITTLE_BALL_DIAMETER / 2;
            
            // ghost ball (rotated one) is the normal ball rotated from minus angle the racket has been rotated
            rotatedBallCircle.x = rotOrigin.x + (ballCircle.x - rotOrigin.x) * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                    + (ballCircle.y - rotOrigin.y) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
            rotatedBallCircle.y = rotOrigin.y - (ballCircle.x - rotOrigin.x) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                    + (ballCircle.y - rotOrigin.y) * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
            
            // the ghost ball has also its speed vector rotated
            rotatedBallSpeed.x = (m_ball->getX_speed()) * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                    + (m_ball->getY_speed()) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
            rotatedBallSpeed.y = -(m_ball->getX_speed()) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                    + (m_ball->getY_speed()) * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
            
            // definition of the ghost Circle ball
            rotatedBallRect.x = rotatedBallCircle.x - rotatedBallCircle.radius;
            rotatedBallRect.y = rotatedBallCircle.y - rotatedBallCircle.radius;
            rotatedBallRect.w = rotatedBallCircle.radius * 2;
            rotatedBallRect.h = rotatedBallCircle.radius * 2;
            
            //To display the ghost ball :
            //SDL_RenderDrawLine(renderer, rotatedBallCircle.x, rotatedBallCircle.y, rotatedBallCircle.x + 10 * rotatedBallSpeed.x, rotatedBallCircle.y + 10 * rotatedBallSpeed.y);
            //SDL_RenderCopy(renderer, this->ballTexture, NULL, &rotatedBallRect);
            
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
            squareDistance = (rotatedBallCircle.x - closest.x) * (rotatedBallCircle.x - closest.x)
                    + (rotatedBallCircle.y - closest.y) * (rotatedBallCircle.y - closest.y);
            
            // if distance between the ghost ball and the closest point on the non rotated racket
            // is lower than the ball radius
            if (squareDistance <= rotatedBallCircle.radius * rotatedBallCircle.radius) {
                contact = true; // Collision !
                m_rackets[MIDDLE]->setMiddleRacketRotationActivated(false); // rotation is deactivated during a brief moment
            } else
                contact = false; // othewise, no collision
            
            if (squareDistance >= rotatedBallCircle.radius * rotatedBallCircle.radius + 1000) {
                // if the distance is greater than radius + an arbitrary constant
                m_rackets[MIDDLE]->setMiddleRacketRotationActivated(true); // we reactivate rotation
            }
        }
        
        
        
        if (contact) 
        {
            if (m_score > 0)
                --m_score;
            if (MIDDLE == closestRacketIndex)
            {
                AB = Collision::rebound(racketRect, rotatedBallRect, rotatedBallSpeed);// selection of the rebound segment
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
                newSpeed.x = newSpeedBeforeReturnRotation.x * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                        - (newSpeedBeforeReturnRotation.y) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
                newSpeed.y = (newSpeedBeforeReturnRotation.x) * sin(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180)
                        + (newSpeedBeforeReturnRotation.y) * cos(3.14 * m_rackets[MIDDLE]->getMiddleRacketAngle() / 180);
            }
            else
            {
                
                A.x = m_rackets[closestRacketIndex]->getReboundSurface().A.x;
                A.y = m_rackets[closestRacketIndex]->getReboundSurface().A.y;
                B.x = m_rackets[closestRacketIndex]->getReboundSurface().B.x;
                B.y = m_rackets[closestRacketIndex]->getReboundSurface().B.y;
                C.x = ballCircle.x;
                C.y = ballCircle.y;
                
                normal = Collision::getNormal(A, B, C); // get the normal
                newSpeed = Collision::calculateVectorV2(oldSpeed, normal); //calculate new speed vector
                
                // get the ratio AC/AB, C being the contact point of the ball on a AB segment
                tempRatio = getRacketContactRatio(A, B, C);
                
                // empiric and arbitrary speed modification following racket's speed
                coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + m_rackets[closestRacketIndex]->getRacketSpeed();
                
                newSpeed = m_rackets[closestRacketIndex]->modifySpeedVector(newSpeed, coeffSpeedModif, tempRatio, squareNormSpeed);
                
                
            }
            //
            if (isnan(newSpeed.x)) // should not occur but some collision bugs persist
                newSpeed.x = -oldSpeed.x;
            if (isnan(newSpeed.y))
                newSpeed.y = -oldSpeed.y;
            m_ball->setX_speed(newSpeed.x);
            m_ball->setY_speed(newSpeed.y);
            m_ball->setX(m_ball->getX() + newSpeed.x);
            m_ball->setY(m_ball->getY() + newSpeed.y);
        }
        
    }
    
    if (m_score != m_pastScore) 
    {
        m_scoreText->changeText(to_string(m_score));
        m_pastScore = m_score;
        
    }
    buildBrickMap();
    m_menu->draw();
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
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            m_brickMap[i][j]->draw();
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
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER ; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            switch (m_brickMap[i][j]->GetResistance()) {
                case 0:
                case 4:
                    break;
                case 1:
                case 2:
                case 3:
                    m_won = false;
                    break;
            }
        }
    }
    if (true == m_won) {
        m_won = false;
        m_stateContext.setScore(m_score);
        m_stateContext.setMenuType(SCORE_MENU);
        m_game.setCurrentState(MENU_STATE);
        
    }
    
}

/* Function : clearBrickMap
 * 
 * Goal : set m_brickmap resistances to 0
 */
void GameState::clearBrickMap() {
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            m_brickMap[i][j]->SetResistance(0);
        }
    }
}




void GameState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;
    vector<IKeyboardListener*>::const_iterator it;
    for (it = m_menu->getKeyboardListenerVector().begin(); it != m_menu->getKeyboardListenerVector().end(); ++it) 
        (*it)->onKeyDown(evt);
}

void GameState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    //getProperRacket(evt->key.keysym.sym)->decelerate();
    vector<IKeyboardListener*>::const_iterator it;
    for (it = m_menu->getKeyboardListenerVector().begin(); it != m_menu->getKeyboardListenerVector().end(); ++it) 
        (*it)->onKeyUp(evt);
    
    // A listener object is missing here for the escape event. It can not be the state 
    // itself as its Menu is an attribute, and destroying the state implies destruction of
    // the Menu and what it contains. Escape event is thus handled here for the moment.
    if (SDLK_ESCAPE == evt->key.keysym.sym) {
        m_levelSelected = false;
        m_ball->setLaunched(false);
        m_ball->setPlaced(false);
        m_rackets[MIDDLE]->setMiddleRacketAngle(0);
        
        m_stateContext.setMenuType(LEVEL_CHOICE_MENU);
        m_game.setCurrentState(MENU_STATE);
    }
}

void GameState::onQuit() {
    m_game.stop();
}


void GameState::setRenderDrawColor(SDL_Renderer * renderer, Color* color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, SDL_ALPHA_OPAQUE);
}

void GameState::onEvent(SDL_Event* evt) {
    
    switch (evt->type) {
        case SDL_QUIT: 
            onQuit();
            break;
        case SDL_KEYDOWN: 
            onKeyDown(evt);
            break;
        case SDL_KEYUP: 
            onKeyUp(evt);
            break;
        default:
            break;
    }
}


int GameState::getIntSqrDistance(int xa, int ya, int xb, int yb) {
    return ((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb));
}

Racket * GameState::getProperRacket(SDL_Keycode keyId) {
    if (keyId == SDLK_LEFT || keyId == SDLK_RIGHT) {
        return m_rackets[BOTTOM];
    } else if (keyId == SDLK_UP || keyId == SDLK_DOWN) {
        return m_rackets[RIGHT];
    } else if (keyId == SDLK_z || keyId == SDLK_s) {
        return m_rackets[LEFT];
    } else if (keyId == SDLK_q || keyId == SDLK_d) {
        return m_rackets[TOP];
    } else
        return m_rackets[BOTTOM];
    
}

void GameState::updateRacketsPositions() {
    
    vector<IKeyboardListener*>::const_iterator it;
    for (it = m_menu->getKeyboardListenerVector().begin(); it != m_menu->getKeyboardListenerVector().end(); ++it) 
        (*it)->onKeyPressed(m_keys);
}

T_RACKET_POSITION GameState::getClosestRacket()  {
    map<T_RACKET_POSITION,int> distanceTable;
    using pair_type = decltype(distanceTable)::value_type;
    
    distanceTable[BOTTOM] = getIntSqrDistance(m_ball->getX(),m_ball->getY(), m_rackets[BOTTOM]->getX(), m_rackets[BOTTOM]->getY() );
    distanceTable[TOP] = getIntSqrDistance(m_ball->getX(),m_ball->getY(), m_rackets[TOP]->getX(), m_rackets[TOP]->getY() ); 
    distanceTable[LEFT] = getIntSqrDistance(m_ball->getX(),m_ball->getY(), m_rackets[LEFT]->getX(), m_rackets[LEFT]->getY() ); 
    distanceTable[RIGHT] = getIntSqrDistance(m_ball->getX(),m_ball->getY(), m_rackets[RIGHT]->getX(), m_rackets[RIGHT]->getY() ); 
    distanceTable[MIDDLE] = getIntSqrDistance(m_ball->getX(),m_ball->getY(), m_rackets[MIDDLE]->getX(), m_rackets[MIDDLE]->getY() ); 
    
    auto pr = std::min_element
    (
    std::begin(distanceTable), std::end(distanceTable),
            [] (const pair_type & p1, const pair_type & p2) {
                return p1.second < p2.second;
            }
            );
            return pr->first;
            
}


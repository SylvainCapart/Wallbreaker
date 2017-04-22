/* 
 * File:   Game.cpp
 * Author: Sylvain
 * 
 * Main class of the program, handling gameplay, menus, graphics.
 * 
 * TODO v0.2 : feature ONGOING
 * Create a proper Menu class, that can handle a proper menu, with choices 
 * highlighted, nice graphics, etc. Present menu (and end screen) are just images.
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
 * TODO v0.3 : implement the pattern state for Game
 * 
 * TODO v0.3 : implement virtual interface IDrawable for Menu
 * 
 * TODO v0.3 : add boost smart ptr
 * 
 * TODO v0.3 : minor corrections : hard values, ++i, m_attributes, check switch cases, reverse ==
 * 
 * TODO : optimisation
 * Decorrelate the run, draw and collision activities in different threads. For
 * moment they are all in the run() thread.
 * 
 * TODO v0.2 : handle bugs occuring when the ball and the middle racket collide : ball disappears.
 * 
 * DONE v0.1 : debug
 * Handle a bug causing the ball to perform multiple hits on the same bricks, or 
 * to perform several hits on different bricks.
 * 
 * TODO v0.2 : debug
 * Handle a bug causing the ball to not escape the middle racket when the player
 * make it rotate and a collision occurs. Therefore the ball after the rebound, 
 * is considered to be still in collision with the segment and a new normal vector 
 * is chosen towards the inside of the racket. The ball escapes when it reaches the 
 * corner.
 * 
 * DONE v0.1 : debug
 * Handle a bug causing the end screen to be displayed when not needed.
 * 
 * TODO v0.2 : improvement
 * Handle ball collision on the sides of the rackets, which causes the ball to 
 * enter inside the racket.
 * 
 * Created on 5 janvier 2017, 10:16
 */

#include "Game.h"

using namespace std;

Game::Game()
: frameSkip(0), running(0), window(NULL), renderer(NULL), menu(NULL) {

    ball = new Ball();
    currentLevel = new Level();

    
    menuDrawn = false;
    levelSelected = false;
    won = false;
    wonDrawn = false;
    middleRacketPresent = true;
    middleRacketRotationActivated = true;
    mouseIsMoving = true;
    mouseX = 0;
    mouseY = 0;
    mouseButtonUp = false;
    mouseButtonDown = false;

    collisionInhibition = 0;
    collisionActivated = true;

    cobalt = new Color(34, 66, 124);
    darkCobalt = new Color(23, 45, 84);
    red = new Color(255, 0, 0);
    blueGreen = new Color(18, 179, 174);
    darkBlueGreen = new Color(9, 45, 45);
    yellow = new Color(247, 236, 133);
    paleOrange = new Color(255, 178, 102);
    green = new Color(0, 204, 0);
    black = new Color(0, 0, 0);


}

void Game::initRackets() {

    rackets[BOTTOM] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET, HORIZONTAL);
    rackets[TOP] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, RACKET_HEIGHT / 2, HORIZONTAL);
    rackets[LEFT] = new Racket(RACKET_HEIGHT / 2, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2, VERTICAL);
    rackets[RIGHT] = new Racket(WINDOW_WIDTH - RACKET_HEIGHT / 2 - MARGIN_BOTTOM_RACKET, WINDOW_HEIGHT / 2 - RACKET_WIDTH / 2 - MARGIN_BOTTOM_RACKET, VERTICAL);
    middleRacketAngle = 0;

    if (middleRacketPresent) {
        rackets[MIDDLE] = new Racket(WINDOW_WIDTH / 2 - RACKET_WIDTH / 2, WINDOW_HEIGHT / 2 - RACKET_HEIGHT / 2, HORIZONTAL);
    }

}

Game::~Game() {
    this->stop();
}

/* Function : start
 * Input : none
 * Output : none
 * Goal : creates the SDL windows
 *        loads images into SDL_Texture
 * 
 * TODO : improvement
 * Simplify by having the ball just follow the center of the racket, which shall be blocked at the right position.
 */

void Game::start() {
    gameState = MENU;

    int flags = SDL_WINDOW_SHOWN;
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        cout << "Error using SDL_Init()" << endl;
        return;
    }

    if (TTF_Init() < 0) {
        cout << "Error using TTF_Init()" << endl;
        return;
    }

    if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &window, &renderer)) {
        return;
    }

    menu = new Menu(renderer);


    ballTexture = IMG_LoadTexture(renderer, "img/ball2.png");
    if (ballTexture == NULL)
        cout << "Error charging ball graphics" << endl;


    wonTexture = IMG_LoadTexture(renderer, "img/welldone.jpg");
    if (wonTexture == NULL)
        cout << "Error charging end screen graphics" << endl;

    initRackets();

    this->running = 1;
    run();


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

void Game::draw() {


    SDL_Rect racketRect; // drawing area for a racket
    bool contact = false; // contact ball/racket
    bool brickContact = false; // contact ball/brick



    float tempRatio; // temp variable for speed modifications
    int coeffSpeedModif; // variable for speed modifications
    int racketsNumber; // number of rackets
    Vector rotatedBallSpeed; // speed of the ghost ball used for the collision on
    //the rotated racket
    float squareNormSpeed; // norm speed

    Vector normal, newSpeed, oldSpeed; // vectors used for speed changes
    Point A, B, C; // points used for collision
    SegmentPoints AB; // rebound segment of an obstacle

    Color * screenColor = cobalt;

    oldSpeed.x = ball->getX_speed(); // x-speed before impact
    oldSpeed.y = ball->getY_speed(); // y-speed before impact

    squareNormSpeed = oldSpeed.x * oldSpeed.x + oldSpeed.y * oldSpeed.y; // norm

    SDL_Rect brickRect; // brick rectangle drawing
    brickRect.w = BRICK_WIDTH;
    brickRect.h = BRICK_HEIGHT;

    SDL_Rect ballRect; // rectangle/square surrounding the ball
    ballRect.x = ball->getX();
    ballRect.y = ball->getY();
    ballRect.w = ball->getW();
    ballRect.h = ball->getH();


    Circle ballCircle; // ball described by its center coordinates and radius
    ballCircle.radius = LITTLE_BALL_DIAMETER / 2;
    ballCircle.x = ball->getX() + ballCircle.radius;
    ballCircle.y = ball->getY() + ballCircle.radius;

    // place the ball
    if (!ball->isPlaced() && !ball->isLaunched()) {
        placeBall();
    }

    // setting the screen color
    SDL_SetRenderDrawColor(renderer, screenColor->r, screenColor->g, screenColor->b, SDL_ALPHA_OPAQUE); // setting background color
    SDL_RenderClear(renderer); // clearing present renderer

    // check collision of the ball with bricks
    for (int i = 0; i < BRICK_AREA_X_NUMBER && !brickContact; i++) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER && !brickContact; j++) {
            switch (brickMap[i][j]->GetResistance()) {
                case 0:
                    break;

                case 1:
                case 2:

                    brickRect.x = brickMap[i][j]->GetX();
                    brickRect.y = brickMap[i][j]->GetY();
                    brickContact = Collision::collisionCircleAABB(ballCircle, brickRect); // true if collision

                    if (brickContact == true) {
                        AB = ball->rebound(brickRect); // getting segment AB where we want the rebound
                        //AB = Collision::rebound(brickRect, ballRect, speedBallTest);
                        if (AB.A.x == 0 && AB.A.y == 0 && AB.B.x == 0 && AB.B.y == 0) {
                            // this case should not happen and is here for testing reasons
                            break;
                        }
                        A.x = AB.A.x; // segment AB where the ball will do the rebound
                        A.y = AB.A.y;
                        B.x = AB.B.x;
                        B.y = AB.B.y;
                        C.x = ball->getXCenter();
                        C.y = ball->getYCenter();

                        normal = Collision::getNormal(A, B, C); // we will the the projection of C on the AB segment, and then the normal
                        // vector to the segment on this projection

                        newSpeed = Collision::calculateVectorV2(oldSpeed, normal); // getting new speed following the incoming vector
                        // speed, regarding the normal vector

                        ball->setX_speed(newSpeed.x);
                        ball->setY_speed(newSpeed.y); //

                        --(*brickMap[i][j]); // decreasing resistance 1->0 or 2->1 after collision 

                    }
                    break;

            }

        }
    }

    // ****** Render bricks ******

    // Render the bricks after collision is made
    for (int i = 0; i < BRICK_AREA_X_NUMBER; i++) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; j++) {
            switch (brickMap[i][j]->GetResistance()) {
                case 0:
                    break;

                case 1:
                    brickRect.x = brickMap[i][j]->GetX();
                    brickRect.y = brickMap[i][j]->GetY();

                    fillRectGame(&brickRect, *yellow); // sets the color + draw the rectangle with it

                    setRenderDrawColor(renderer, darkCobalt);
                    SDL_RenderDrawLine(renderer, brickRect.x, brickRect.y, brickRect.x + brickRect.w, brickRect.y);
                    SDL_RenderDrawLine(renderer, brickRect.x + brickRect.w, brickRect.y, brickRect.x + brickRect.w, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(renderer, brickRect.x + brickRect.w, brickRect.y + brickRect.h, brickRect.x, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(renderer, brickRect.x, brickRect.y + brickRect.h, brickRect.x, brickRect.y);

                    break;

                case 2:
                    brickRect.x = brickMap[i][j]->GetX();
                    brickRect.y = brickMap[i][j]->GetY();

                    fillRectGame(&brickRect, *paleOrange);

                    setRenderDrawColor(renderer, darkCobalt);
                    SDL_RenderDrawLine(renderer, brickRect.x, brickRect.y, brickRect.x + brickRect.w, brickRect.y);
                    SDL_RenderDrawLine(renderer, brickRect.x + brickRect.w, brickRect.y, brickRect.x + brickRect.w, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(renderer, brickRect.x + brickRect.w, brickRect.y + brickRect.h, brickRect.x, brickRect.y + brickRect.h);
                    SDL_RenderDrawLine(renderer, brickRect.x, brickRect.y + brickRect.h, brickRect.x, brickRect.y);
                    break;
            }


        }
    }

    SDL_RenderCopy(renderer, this->ballTexture, NULL, &ballRect); // copy the rectangle on the renderer

    // ****** Render the rackets ******
    if (middleRacketPresent) // depends if we want a middle racket or not
        racketsNumber = TOTAL_RACKET_NUMBER;
    else
        racketsNumber = BASE_RACKET_NUMBER;

    Point rotOrigin; // position of middle racket center
    rotOrigin.x = rackets[MIDDLE]->getRacketCenter().x;
    rotOrigin.y = rackets[MIDDLE]->getRacketCenter().y;

    for (int i = 0; i < racketsNumber; i++) {

        if (middleRacketPresent && (i == MIDDLE)) {

            racketRect.x = rackets[MIDDLE]->getX();
            racketRect.y = rackets[MIDDLE]->getY();
            racketRect.w = rackets[MIDDLE]->getW();
            racketRect.h = rackets[MIDDLE]->getH();

            SDL_Texture * myText = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RACKET_WIDTH, RACKET_HEIGHT);

            SDL_SetRenderTarget(renderer, myText);
            SDL_SetRenderDrawColor(renderer, blueGreen->r, blueGreen->g, blueGreen->b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderTarget(renderer, NULL);

            SDL_QueryTexture(myText, NULL, NULL, &racketRect.w, &racketRect.h);

            //render a rectangle rotated by middleRacketAngle
            SDL_RenderCopyEx(renderer, myText, NULL, &racketRect, middleRacketAngle, NULL, SDL_FLIP_NONE);




        } else { // top bottom right and left rendering
            racketRect.x = rackets[i]->getX();
            racketRect.y = rackets[i]->getY();
            racketRect.w = rackets[i]->getW();
            racketRect.h = rackets[i]->getH();
            setRenderDrawColor(renderer, darkBlueGreen);
            SDL_RenderDrawLine(renderer, racketRect.x, racketRect.y, racketRect.x + racketRect.w, racketRect.y);
            SDL_RenderDrawLine(renderer, racketRect.x + racketRect.w, racketRect.y, racketRect.x + racketRect.w, racketRect.y + racketRect.h);
            SDL_RenderDrawLine(renderer, racketRect.x + racketRect.w, racketRect.y + racketRect.h, racketRect.x, racketRect.y + racketRect.h);
            SDL_RenderDrawLine(renderer, racketRect.x, racketRect.y + racketRect.h, racketRect.x, racketRect.y);
            fillRectGame(&racketRect, *blueGreen); // sets the color + draw the rectangle with it
        }



        // ****** Handle racket collision ****** 
        if (i != MIDDLE)
            contact = Collision::collisionCircleAABB(ballCircle, racketRect); // detects collision with the racket
        else {
            Circle rotatedBallCircle; // ghost ball for collision on a non rotated middle racket

            rotatedBallCircle.radius = LITTLE_BALL_DIAMETER / 2;

            // ghost ball (rotated one) is the normal ball rotated from minus angle the racket has been rotated
            rotatedBallCircle.x = rotOrigin.x + (ballCircle.x - rotOrigin.x) * cos(3.14 * middleRacketAngle / 180)
                    + (ballCircle.y - rotOrigin.y) * sin(3.14 * middleRacketAngle / 180);
            rotatedBallCircle.y = rotOrigin.y - (ballCircle.x - rotOrigin.x) * sin(3.14 * middleRacketAngle / 180)
                    + (ballCircle.y - rotOrigin.y) * cos(3.14 * middleRacketAngle / 180);

            // the ghost ball has also its speed vector rotated
            rotatedBallSpeed.x = (ball->getX_speed()) * cos(3.14 * middleRacketAngle / 180)
                    + (ball->getY_speed()) * sin(3.14 * middleRacketAngle / 180);
            rotatedBallSpeed.y = -(ball->getX_speed()) * sin(3.14 * middleRacketAngle / 180)
                    + (ball->getY_speed()) * cos(3.14 * middleRacketAngle / 180);

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
                middleRacketRotationActivated = false; // rotation is deactivated during a brief moment
            } else
                contact = false; // othewise, no collision

            if (squareDistance >= rotatedBallCircle.radius * rotatedBallCircle.radius + 2000) {
                // if the distance is greater than radius + an arbitrary constant
                middleRacketRotationActivated = true; // we reactivate rotation
                collisionActivated = true; // we reactivate collision
                //cout << "COLLISION REACTIVATED " << endl;
            }

            if (contact && collisionActivated) { // if we detect a collision and collision is activated

                Vector newSpeedBeforeReturnRotation;

                // selection of the rebound segment
                AB = Collision::rebound(racketRect, rotatedBallRect, rotatedBallSpeed);

                if (AB.A.x == 0 && AB.A.y == 0 && AB.B.x == 0 && AB.B.y == 0) {
                    //this case is here for testing reasons, it should not happen
                }

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
                newSpeed.x = newSpeedBeforeReturnRotation.x * cos(3.14 * middleRacketAngle / 180)
                        - (newSpeedBeforeReturnRotation.y) * sin(3.14 * middleRacketAngle / 180);
                newSpeed.y = (newSpeedBeforeReturnRotation.x) * sin(3.14 * middleRacketAngle / 180)
                        + (newSpeedBeforeReturnRotation.y) * cos(3.14 * middleRacketAngle / 180);


            }


        }

        // collision with the other rackets 
        if (contact && ball->isLaunched()) {
            if (i == BOTTOM) {
                A.x = rackets[i]->getX(); // AB segment where rebound is done 
                A.y = rackets[i]->getY();
                B.x = A.x + rackets[i]->getW();
                B.y = A.y;
                C.x = ballCircle.x; // C center of the ball during impact
                C.y = ballCircle.y;

                normal = Collision::getNormal(A, B, C);
                newSpeed = Collision::calculateVectorV2(oldSpeed, normal);

                tempRatio = getRacketContactRatio(A, B, C);

                // empiric and arbitrary speed modification following racket's speed
                coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + rackets[i]->getRacketSpeed();

                // setting the new speed. 0.5 because we count from the middle of the racket.
                // if tempratio = 0.5 ( ie hit at the center of the racket ) then 
                // the new speed is not modified and is like a "normal" rebound
                newSpeed.x = newSpeed.x - coeffSpeedModif * (0.5 - tempRatio);

                // the other speed component is defined regarding the first one,
                // so that speed norm remains unchanged.
                newSpeed.y = -sqrt(abs(squareNormSpeed - newSpeed.x * newSpeed.x));

            } else if (i == TOP) {
                // see above the BOTTOM case
                A.x = rackets[i]->getX();
                A.y = rackets[i]->getY() + rackets[i]->getH();
                B.x = A.x + rackets[i]->getW();
                B.y = A.y;
                C.x = ballCircle.x;
                C.y = ballCircle.y;

                normal = Collision::getNormal(A, B, C);
                newSpeed = Collision::calculateVectorV2(oldSpeed, normal);

                tempRatio = getRacketContactRatio(A, B, C);

                coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + rackets[i]->getRacketSpeed();
                newSpeed.x = newSpeed.x - coeffSpeedModif * (0.5 - tempRatio);
                newSpeed.y = sqrt(abs(squareNormSpeed - newSpeed.x * newSpeed.x));

            } else if (i == LEFT) {
                // see above the BOTTOM case
                A.x = rackets[i]->getX() + rackets[i]->getW();
                A.y = rackets[i]->getY();
                B.x = A.x;
                B.y = A.y + rackets[i]->getH();
                C.x = ballCircle.x;
                C.y = ballCircle.y;

                //cout << "LEFT COLLISION, BALL COORDINATE :" << ballCircle << endl;; 

                normal = Collision::getNormal(A, B, C);
                newSpeed = Collision::calculateVectorV2(oldSpeed, normal);

                tempRatio = getRacketContactRatio(A, B, C);

                coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + rackets[i]->getRacketSpeed();
                newSpeed.y = newSpeed.y - coeffSpeedModif * (0.5 - tempRatio);
                newSpeed.x = sqrt(abs(squareNormSpeed - newSpeed.y * newSpeed.y));

            } else if (i == RIGHT) {
                // see above the BOTTOM case
                A.x = rackets[i]->getX();
                A.y = rackets[i]->getY();
                B.x = A.x;
                B.y = A.y + rackets[i]->getH();
                C.x = ballCircle.x;
                C.y = ballCircle.y;
                normal = Collision::getNormal(A, B, C);
                newSpeed = Collision::calculateVectorV2(oldSpeed, normal);

                tempRatio = getRacketContactRatio(A, B, C);

                coeffSpeedModif = getSpeedModifCoeffFromContactRatio(tempRatio) + rackets[i]->getRacketSpeed();
                newSpeed.y = newSpeed.y - coeffSpeedModif * (0.5 - tempRatio);
                newSpeed.x = -sqrt(abs(squareNormSpeed - newSpeed.y * newSpeed.y));

            } else {
                // middle racket already handled
            }
            if (collisionActivated) {

                ball->setX_speed(newSpeed.x);
                ball->setY_speed(newSpeed.y);

                ball->setX(ball->getX() + newSpeed.x);
                ball->setY(ball->getY() + newSpeed.y);
            }
        }

    }

    SDL_RenderPresent(renderer); // displays the renderer where we put all the graphics before

}

/* Function : buildBrickMap
 * 
 * Input : None
 * 
 * Output : None
 * 
 * Goal : displays the brickMap at the screen
 */

void Game::buildBrickMap() {

    SDL_Rect brickRect;
    brickRect.w = BRICK_WIDTH;
    brickRect.h = BRICK_HEIGHT;

    for (int i = 0; i < BRICK_AREA_X_NUMBER; i++) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; j++) {
            switch (brickMap[i][j]->GetResistance()) {
                case 0:
                    break;

                case 1:
                    brickRect.x = (BRICK_AREA_X_ORIGIN_NUMBER + i) * BRICK_WIDTH;
                    brickRect.y = (BRICK_AREA_Y_ORIGIN_NUMBER + j) * BRICK_HEIGHT;
                    fillRectGame(&brickRect, *yellow); // sets the color + draw the rectangle with it
                    break;

                case 2:
                    brickRect.x = (BRICK_AREA_X_ORIGIN_NUMBER + i) * BRICK_WIDTH;
                    brickRect.y = (BRICK_AREA_Y_ORIGIN_NUMBER + j) * BRICK_HEIGHT;
                    fillRectGame(&brickRect, *paleOrange);
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

float Game::getRacketContactRatio(const Point & A, const Point & B, const Point & C) {
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

int Game::getSpeedModifCoeffFromContactRatio(float contactRatio) {

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

void Game::checkBrickPresence() {

    won = true;
    for (int i = 0; i < BRICK_AREA_X_NUMBER; i++) {
        for (int j = 0; j < BRICK_AREA_Y_NUMBER; j++) {
            switch (brickMap[i][j]->GetResistance()) {
                case 0:
                    break;
                case 1:
                    won = false;
                    break;
                case 2:
                    won = false;
                    break;
            }
        }
    }
    if (won == true) {


        gameState = WON;
        levelSelected = false;
        wonDrawn = false;
        won = false;
        ball->setLaunched(false);
        ball->setPlaced(false);

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

void Game::placeBall() {

    initialRacketId = rand() % 4;

    ball->setInitialRacketId(initialRacketId);

    switch (initialRacketId) {
        case BOTTOM:
            ball->setX(rackets[BOTTOM]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            ball->setY(rackets[BOTTOM]->getY() - LITTLE_BALL_DIAMETER);
            break;
        case TOP:
            ball->setX(rackets[TOP]->getX() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            ball->setY(rackets[TOP]->getY() + RACKET_HEIGHT);
            break;
        case LEFT:
            ball->setX(rackets[LEFT]->getX() + RACKET_HEIGHT);
            ball->setY(rackets[LEFT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2 - 0.1);

            break;
        case RIGHT:
            ball->setX(rackets[RIGHT]->getX() - LITTLE_BALL_DIAMETER);
            ball->setY(rackets[RIGHT]->getY() + RACKET_WIDTH / 2 - LITTLE_BALL_DIAMETER / 2);
            break;
    }

    ball->setPlaced(true);
}

void Game::stop() {
    if (NULL != renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    SDL_DestroyTexture(ballTexture);


    if (NULL != window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    TTF_Quit();

    for (int i = 0; i < TOTAL_RACKET_NUMBER; i++)
        delete rackets[i];
    delete ball;
    delete currentLevel;
    
    delete green;
    delete cobalt;
    delete darkCobalt;
    delete red;
    delete black;
    delete blueGreen;
    delete darkBlueGreen;
    delete yellow;
    delete paleOrange;

}

void Game::fillRectGame(SDL_Rect* rc, int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Game::fillRectGame(SDL_Rect* rc, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Game::fpsChanged(int fps) {
    char szFps[ 128 ];
    sprintf(szFps, "%s: %d FPS", "SDL2 Base C++ - Use Arrow Keys / ZQSD SPACE to Move", fps);
    SDL_SetWindowTitle(window, szFps);
}

void Game::onQuit() {
    running = 0;
}

/* Function : run
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : main thread, handling drawings following game state.
 * 
 * TODO v0.2 : optimisation
 * decorrelate from the drawing and collision activities
 */

void Game::run() {
    int past = SDL_GetTicks();
    int now = past, pastFps = past;
    int fps = 0, framesSkipped = 0;

    SDL_Event event;
    while (running) {

        int timeElapsed = 0;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: onQuit();
                    break;
                case SDL_KEYDOWN: onKeyDown(&event);
                    break;
                case SDL_KEYUP: onKeyUp(&event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    cout << "down" << endl;
                    mouseButtonDown = true;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    break;
                case SDL_MOUSEBUTTONUP:
                    cout << "up" << endl;
                    mouseButtonUp = true;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    break;
                case SDL_MOUSEMOTION:
                    cout << "motion" << endl;
                    mouseIsMoving = true;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    break;
            }
        }

        timeElapsed = (now = SDL_GetTicks()) - past;

        if (timeElapsed >= UPDATE_INTERVAL) {
            past = now;

            update();
            notLaunchedBallUpdate();

            if (framesSkipped++ >= frameSkip) {

                if (ball->isLaunched() && (gameState == GAME)) {
                    ball->move();
                }

                if (gameState == GAME) {
                    if (!levelSelected) {
                        currentLevel->loadLevel(brickMap);
                        levelSelected = true;
                    }
                    draw();
                } else if (gameState == MENU)
                {
                    if (NULL == menu)
                    menu = new Menu(renderer);
                
                    drawMenu();
                }

                else if (gameState == WON)
                    drawWon();
                ++fps;
                framesSkipped = 0;
            }
        }
        if (now - pastFps >= 1000) {
            pastFps = now;
            fpsChanged(fps);
            fps = 0;
        }

        SDL_Delay(1);
        mouseIsMoving = false;
        mouseButtonDown = false;
        mouseButtonUp = false;
        //if (gameState == GAME)
        //  checkBrickPresence();

    }
}

void Game::drawMenu() {

    //the efficiency/speed of the list/vector in this case is to be examined

    vector<Button>::iterator it;

    //if (mouseIsMoving || mouseButtonDown || mouseButtonUp) {
    for (it = menu->getButtonList().begin(); it != menu->getButtonList().end(); ++it) {
        if ((*it).detectSelection(mouseX, mouseY)) {
            if (mouseButtonDown ) {
                cout << "button pressed" << endl;
                (*it).setButtonState(PRESSED);
            } else if ( mouseButtonUp && ((*it).getText() == "Start")) {
                currentLevel->setLevelId(1);
                menuDrawn = false;
                gameState = GAME;
                delete menu;
                menu = NULL;
                return;
            } else if ( mouseButtonUp && ((*it).getText() == "Quit")) {
                running = 0;
                delete menu;
                menu = NULL;
                return;
            } else 
                (*it).setButtonState(MOUSE_OVER);
        } else
            (*it).setButtonState(NO_ACTION);
    }
    menu->update();
    //}
}

void Game::drawWon() {
    if (!wonDrawn) {
        SDL_Rect wonRect;
        wonRect.x = 0;
        wonRect.y = 0;
        wonRect.h = WINDOW_HEIGHT;
        wonRect.w = WINDOW_WIDTH;
        SDL_RenderCopy(renderer, this->wonTexture, NULL, &wonRect);
        SDL_RenderPresent(renderer);
        wonDrawn = true;
    }
}

/* Function : update
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : performs different actions following game state : rackets movements,
 * level choices, launch ball
 */

void Game::update() {
    switch (gameState) {
        case GAME:
            if (keys[SDLK_LEFT]) {
                rackets[BOTTOM]->moveLeft();
            }
            if (keys[SDLK_RIGHT]) {
                rackets[BOTTOM]->moveRight();
            }
            if (keys[SDLK_UP]) {
                rackets[RIGHT]->moveUp();
            }
            if (keys[SDLK_DOWN]) {
                rackets[RIGHT]->moveDown();
            }
            if (keys[SDLK_z]) {
                rackets[LEFT]->moveUp();
            }
            if (keys[SDLK_q]) {
                rackets[TOP]->moveLeft();
            }
            if (keys[SDLK_s]) {
                rackets[LEFT]->moveDown();
            }
            if (keys[SDLK_d]) {
                rackets[TOP]->moveRight();
            }
            if (keys[SDLK_SPACE]) {
                if (ball->isPlaced() && !ball->isLaunched()) {
                    ball->setInitialSpeed(initialRacketId);
                    ball->launch();
                }
                if (middleRacketRotationActivated) {

                    middleRacketAngle = (middleRacketAngle + MID_RACKET_ANGLE_SPEED) % ANGLE_MODULO;

                }



            }
            break;

        case MENU:


            break;

        case WON:
            break;


    }
}

/* Function : notLaunchedBallUpdate
 * 
 * Input : none
 * 
 * Output : none
 * 
 * Goal : the ball follows the center of the racket
 */

void Game::notLaunchedBallUpdate() {

    if (gameState == GAME) {
        if (!ball->isLaunched()) {
            switch (initialRacketId) {
                case BOTTOM:
                    ball->setX(rackets[BOTTOM]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2);
                    break;
                case RIGHT:
                    ball->setY(rackets[RIGHT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2);
                    break;
                case LEFT:
                    ball->setY(rackets[LEFT]->getRacketCenter().y - LITTLE_BALL_DIAMETER / 2);
                    break;
                case TOP:
                    ball->setX(rackets[TOP]->getRacketCenter().x - LITTLE_BALL_DIAMETER / 2);
                    break;
            }
        }

    }
}

void Game::onKeyDown(SDL_Event* evt) {
    keys[ evt->key.keysym.sym ] = 1;
}

void Game::onKeyUp(SDL_Event* evt) {
    keys[ evt->key.keysym.sym ] = 0;

    //if a racket is returned of this key up, decelerate it
    getProperRacket(evt->key.keysym.sym)->decelerate();

    if (evt->key.keysym.sym == SDLK_ESCAPE) {
        switch (gameState) {
            case GAME:
                gameState = MENU;
                levelSelected = false;
                menuDrawn = false;
                ball->setLaunched(false);
                ball->setPlaced(false);
                initRackets();
                break;

            case WON:
                gameState = MENU;
                menuDrawn = false;
                levelSelected = false;
                ball->setLaunched(false);
                ball->setPlaced(false);
                won = false;
                wonDrawn = true;
                break;

            case MENU:
                running = 0;
                break;
        }
    }



}

Racket * Game::getProperRacket(SDL_Keycode keyId) {
    if (keyId == SDLK_LEFT || keyId == SDLK_RIGHT) {
        return rackets[BOTTOM];
    } else if (keyId == SDLK_UP || keyId == SDLK_DOWN) {
        return rackets[RIGHT];
    } else if (keyId == SDLK_z || keyId == SDLK_s) {
        return rackets[LEFT];
    } else if (keyId == SDLK_q || keyId == SDLK_d) {
        return rackets[TOP];
    } else
        return rackets[BOTTOM];

}

void Game::setRenderDrawColor(SDL_Renderer * renderer, Color* color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, SDL_ALPHA_OPAQUE);
}


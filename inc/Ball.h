/* 
 * File:   Ball.h
 * Author: Sylvain
 *
 * Created on 6 janvier 2017, 18:46
 */

#ifndef BALL_H
#define	BALL_H

#include "IDrawable.h"
#include "Racket.h"

class Ball: public IDrawable, public IKeyboardListener {
public:
    /// \param renderer : where to draw the ball
    /// \param  textureFileName : name of image file ("img/something.png")
    Ball(SDL_Renderer * renderer, const char * textureFileName); // const char * to avoid initializing a string first
    virtual ~Ball();
    
    /// \param h : height (int) of the SDL_Rect corresponding to the ball
    void setH(int h) {this->m_h = h;}
    
    /// \return : height (int) of the SDL_Rect corresponding to the ball
    int getH() const {return m_h;}

    /// \param w : width (int) of the SDL_Rect corresponding to the ball
    void setW(int w) {this->m_w = w;}
    
    /// \return : width (int) of the SDL_Rect corresponding to the ball
    int getW() const {return m_w;}
    
    /// \param launched : setting (boolean) if the ball is launched (it moves, and not on a racket) or not (it doesn't move, still on a racket)
    void setLaunched(bool launched) {this->m_launched = launched;}
    
    /// \return : boolean, true if the ball is launched (it moves, and not on a racket) or not (it doesn't move, still on a racket)
    bool isLaunched() const {return m_launched;}

    /// \param placed : setting (boolean) if the ball position is initialized on a racket or not
    void setPlaced(bool placed) {this->m_placed = placed;}
    
    /// \return : boolean, true is the ball is placed on a racket, or false if not
    bool isPlaced() const {return m_placed;}

    /// \param y : setting y (float) coordinate of the ball
    void setY(float y) {this->m_y = y;}
    
    /// \return : y coordinate (float) of the ball
    float getY() const {return m_y;}

    /// \param x : setting x (float) coordinate of the ball
    void setX(float x) {this->m_x = x;}
    
    /// \return : x (float) coordinate of the ball
    float getX() const {return m_x;}
    
    /// \param initialRacketId : int, ID of one of the rackets
    void setInitialRacketId(T_RACKET_POSITION initialRacketId) {this->m_initialRacketId = initialRacketId;}

    /// \goal : triggers the launch of the ball while indicating it is not placed anymore on the racket
    void launch();
    
    /// \goal : updates the position of the ball following its speed.
    ///         checks speed limits
    ///         checks position limits and reinitiate the ball if it goes past the borders
    void move();
    
    /// \goal : following the racket, give the ball an orthogonal speed;
    void setInitialSpeed();

    /// \return : float, y component of the ball speed
    float getY_speed() const {return m_ySpeed;}
    
    /// \return : float, x component of the ball speed
    float getX_speed() const {return m_xSpeed;}
    
    /// \param y_speed : setting y speed of the ball, float
    void setY_speed(float y_speed);
    
    /// \param x_speed : setting x speed of the ball, float
    void setX_speed(float x_speed);

    /// \goal : return y coordinate of the ball center 
    float getYCenter() const {return m_yCenter;}
    
    /// \goal : return x coordinate of the ball center 
    float getXCenter() const {return m_xCenter;}

    /// \goal : IDrawable method : draw the object
    void draw() const;

    /// \parameter racketPosition : when the ball is not launched, side of the racket
    /// where the ball is placed
    /// \parameter rackets : pointer to a table containing the rackets data
    /// \goal : update ball position
    void update(T_RACKET_POSITION racketPosition, Racket * rackets[TOTAL_RACKET_NUMBER]);

    /// \parameter rackets : pointer to a table containing the rackets data
    /// \returns the side of the chosen racket as a T_RACKET_POSITION, among the 4
    /// base rackets
    /// \goal : place the ball on a random racket
    T_RACKET_POSITION placeBall(Racket * rackets[TOTAL_RACKET_NUMBER]);
    
    void setBallRect(const SDL_Rect & ballRect);
    SDL_Rect getBallRect() const;
    
    virtual void onKeyDown(SDL_Event* evt);
    virtual void onKeyUp(SDL_Event* evt);
    virtual void onKeyPressed(std::map<int, int>& keyMap);



    
    
private:
    int m_x; //X origin of the ball drawing (same origin as the corresponding SDL_Rect)
    int m_y; //Y origin of the ball drawing (same origin as the corresponding SDL_Rect)
    // DONE comment : relevancy of float should be here examined
    int m_w; // Width of the drawing area
    int m_h; // Height of the drawing area
    SDL_Rect m_ballRect;
    
    float m_xCenter; // X center component 
    float m_yCenter; // Y center component
    int m_r;         // ball radius
    float m_xSpeed; // X speed component
    float m_ySpeed; // Y speed component
    bool m_launched; // ball launched from the racket or not
    bool m_placed; // ball placed on a racket or not
    T_RACKET_POSITION m_initialRacketId; // racket where the ball shall be placed
    
    SDL_Renderer * m_renderer;
    SDL_Texture * m_ballTexture; // texture of the ball (its image)

};


#endif	/* BALL_H */


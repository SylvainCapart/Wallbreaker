/* 
 * File:   Menu.h
 * Author: Sylvain
 *
 * Created on 9 avril 2017, 11:57
 */

#ifndef MENU_H
#define	MENU_H

#include "IDrawable.h"
#include "IKeyboardListener.h"
#include "IMouseListener.h"
#include <vector>
#include "Button.h"
#include "Ball.h"
#include "Racket.h"
#include "RotatingRacket.h"

class Menu : public IDrawable{  // Menu manages the IDrawables/IKeyboardListeners/IMouseListener :
    // their creation, destruction and the drawing => factory pattern or almost (different methods because 
    // different arguments for each drawing)
    
public:
    /// \param renderer : renderer where we draw the menu
    /// \goal : support and automate the drawing of IDrawable
    /// \comment : the class started as a Menu, and acts now more like a IDrawable manager.
    /// It is also used by GameState to draw non-menu objects like balls or rackets
    Menu(SDL_Renderer * renderer);
    virtual ~Menu();
    
    /// \goal : draw all IDrawable inside m_drawableVector
    void draw() const;

    /// \goal : getter for the button and drawable vectors
    std::vector<IDrawable*>& getDrawableList() {return m_drawableVector;}
    std::vector<IMouseListener*>& getMouseListenerVector() {return m_mouseListenerVector;}
    std::vector<IKeyboardListener*>& getKeyboardListenerVector() {return m_keyboardListenerVector;}
    
    /// \goal : set the texture of the background
    void setMenuTexture(SDL_Texture* menuTexture);
    
    /// \param font : font of the text
    /// \param text : text to write as a const char *
    /// \param color : color of the text
    /// \param fx, fy, fw, fh : position of the origin and size of the text, relative to window size
    /// \return : the Text that was created
    /// \goal : create a text in this Menu and push it in the IDrawable vector
    /// \comment : const char * to avoid initializing a string first.
    Text * createText(TTF_Font * font, const char * text, const Color & color, float fx, float fy, float fw, float fh); 
    
    /// \param buttonId : id of the button, to identify it when it is clicked
    /// \param buttonImage : pointer to a table of button images containing BUTTON_STATES_NUMBER images
    /// \param font : pointer to the font of the button text
    /// \param text : button text as a const char * 
    /// \param color : color of the button text 
    /// \param fx, fy, fw, fh : position of the origin and size of the button, relative to window size
    /// \return : the Button that was created
    /// \goal : create a Button in this Menu and push it in the IDrawable and Button vectors
    /// \comment : const char * to avoid initializing a string first.
    Button * createButton(const MOUSE_LISTENER_ID buttonId,SDL_Texture * buttonImage[BUTTON_STATES_NUMBER],
            TTF_Font * font, const char * text, const Color & color, float fx, float fy, float fw, float fh);

    /// \param textureFileName : texture aka image of the ball
    /// \return : the Ball that was created
    /// \goal : create a Ball in this Menu and push it in the IDrawable vector.
    /// Position is initialized afterwards
    Ball * createBall(const char * textureFileName);
    
    /// \param x,y : position of the origin of SDL_rect of the racket
    /// \param direction : horizontal or vertical racket
    /// \param fillColor : color of the racket
    /// \param borderColor : color of its borders
    /// \param firstBind, secondBind : id of the keys responding for this racket
    /// \return : a pointer to the Racket that was created
    /// \goal : create a Racket in this Menu and push it in the IDrawable and IKeyboardListener vectors
    Racket * createRacket(int x, int y, T_ORIENTATION direction, const Color & fillColor, const Color & borderColor, 
            T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition);
    
    /// \param x,y : position of the origin of SDL_rect of the racket
    /// \param fillColor : color of the racket
    /// \param borderColor : color of its borders
    /// \param firstBind, secondBind : id of the keys responding for this racket
    /// \return : a pointer to the Racket that was created
    /// \goal : create a Racket in this Menu and push it in the IDrawable and IKeyboardListener vectors
    RotatingRacket * createRotatingRacket(int x, int y, const Color & fillColor, const Color & borderColor,
            T_KEY_BIND firstBind, T_KEY_BIND secondBind, T_RACKET_POSITION racketPosition);
    
private:
    SDL_Texture * m_menuTexture; // menu graphics background
    std::vector<IDrawable*> m_drawableVector; // vector of drawable
    std::vector<IMouseListener*> m_mouseListenerVector; //vector of buttons, could be replaced by a vector of IMouseListener*
    std::vector<IKeyboardListener*> m_keyboardListenerVector;
    SDL_Renderer * m_renderer; // renderer where we draw graphics
};

#endif	/* MENU_H */


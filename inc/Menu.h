/* 
 * File:   Menu.h
 * Author: Sylvain
 *
 * Created on 9 avril 2017, 11:57
 */

#ifndef MENU_H
#define	MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <list>
#include <vector>
#include "Button.h"
#include "Text.h"
#include "IDrawable.h"



class Menu : public IDrawable{
public:
    /// \param renderer : renderer where we draw the menu
    Menu(SDL_Renderer * renderer);
    virtual ~Menu();
    
    /// \goal : draw all IDrawable inside m_drawableVector
    virtual void draw() const;
    
    /// \goal : getter for the button and drawable vectors
    std::vector<IDrawable*>& getDrawableList();
    std::vector<Button*>& getButtonList();
    
    /// \goal : set the texture of the background
    void setMenuTexture(SDL_Texture* menuTexture);

    /// \param x
    /// \param y 
    /// \return : true if (x,y) is in the button rectangle
    /// \comment : not used for the moment, could be linked to the detection of subelements
    virtual bool detectSelection(int x, int y) const {return false;}

    virtual const int getDrawableId() const {return 1;}
    virtual DRAWABLE_TYPE getDrawableType() const {return BUTTON_TYPE;}

    /// \param buttonState : state of the button (PRESSED, MOUSE_OVER, NO_ACTION)
    /// \goal : change button state, ie its image
    virtual void setButtonState(BUTTON_STATE buttonState) const {}
    

    
private:
    SDL_Texture * m_menuTexture; // menu graphics background
    std::vector<IDrawable*> m_drawableVector; // vector of drawable
    std::vector<Button*> m_buttonVector; //vector of buttons
    SDL_Renderer * m_renderer; // renderer where we draw graphics
 

};

#endif	/* MENU_H */


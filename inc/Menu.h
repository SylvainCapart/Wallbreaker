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



class Menu {
public:
    Menu(SDL_Renderer * renderer);
    virtual ~Menu();
    
    void update();
    void init();
    void drawTextBlended(TTF_Font * font, const std::string & message, const SDL_Color & color, int x, int y, int w, int h);

    vector<Button> getButtonList() const {
        return buttonVector;
    }

    
private:
    SDL_Texture * menuTexture; // menu graphics background

    SDL_Texture * buttonImage[3];
    SDL_Renderer * renderer;
    std::vector<Button> buttonVector;
    int buttonInterval;
    
    TTF_Font* titleFont;
    TTF_Font* infoFont;
    
    

};

#endif	/* MENU_H */


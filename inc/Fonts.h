/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Fonts.h
 * Author: user
 *
 * Created on 5 juillet 2017, 13:51
 */

#ifndef FONTS_H
#define FONTS_H

#include <SDL_ttf.h>

class Fonts {
public:

    virtual ~Fonts();
    static void openAllFonts();
    static void closeAllFonts();
    static  TTF_Font * st_numberFont; 
    static  TTF_Font * st_arrowFont;
    static  TTF_Font * st_infoFont;
    static  TTF_Font * st_titleFont;
private:
    
};



#endif /* FONTS_H */


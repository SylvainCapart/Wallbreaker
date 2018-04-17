/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Fonts.cpp
 * Author: user
 * 
 * Created on 5 juillet 2017, 13:51
 */

#include "Fonts.h"
#include <iostream>

using namespace std;

TTF_Font * Fonts::st_numberFont;
TTF_Font * Fonts::st_arrowFont;
TTF_Font * Fonts::st_infoFont;
TTF_Font * Fonts::st_titleFont;

Fonts::~Fonts() {
}

void Fonts::openAllFonts() {
    st_numberFont = TTF_OpenFont("font/Stoehr Numbers.ttf", 124);
    if (!st_numberFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
    
    st_titleFont = TTF_OpenFont("font/play the game.ttf", 124);
    if (!st_titleFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
    
    st_infoFont = TTF_OpenFont("font/LinLibertine_R.ttf", 124);
    if (!st_infoFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
    
    st_arrowFont = TTF_OpenFont("font/Arrows.ttf", 124);
    if (!st_arrowFont) {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
    }
}


void Fonts::closeAllFonts() {
    TTF_CloseFont(st_arrowFont);
    TTF_CloseFont(st_numberFont);
    TTF_CloseFont(st_infoFont);
    TTF_CloseFont(st_titleFont);
    
}


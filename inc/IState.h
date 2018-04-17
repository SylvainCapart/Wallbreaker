/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IState.h
 * Author: user
 *
 * Created on 23 avril 2017, 14:46
 * 
 * This class is the interface (virtual methods) allowing the state pattern implementation.
 * 
 */

#ifndef ISTATE_H
#define ISTATE_H

#include <SDL.h>

//class Game;

class IState  {
public:
    virtual ~IState() {}
    virtual void draw() = 0;
    virtual void onQuit() = 0;
    virtual void onEvent(SDL_Event* evt) = 0;
    virtual void init() = 0;
private:
    
};

#endif /* ISTATE_H */


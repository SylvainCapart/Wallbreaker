/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IKeyboardListener.h
 * Author: user
 *
 * Created on 6 juillet 2017, 17:01
 */

#ifndef IKEYBOARDLISTENER_H
#define IKEYBOARDLISTENER_H

#include "IDrawable.h"
#include <map>

class IKeyboardListener {
public:
    virtual ~IKeyboardListener() {}
    virtual void onKeyUp(SDL_Event * evt) = 0;
    virtual void onKeyDown(SDL_Event * evt) = 0;
    virtual void onKeyPressed(std::map<int,int> & keyMap) = 0;
    virtual void draw() const = 0;
    

private:

};

#endif /* IKEYBOARDLISTENER_H */


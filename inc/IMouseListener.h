/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IMouseListener.h
 * Author: user
 *
 * Created on 30 juin 2017, 17:47
 */

#ifndef IMOUSELISTENER_H
#define IMOUSELISTENER_H

#include <SDL.h>

#include "IDrawable.h"


class IMouseListener {
public:
    virtual ~IMouseListener() {}
    virtual void onClickUp(SDL_Event * evt) = 0;
    virtual void onClickDown(SDL_Event * evt) = 0;
    virtual void onMouseMotion(SDL_Event * evt) = 0;
    virtual bool detectSelection(int x, int y) const = 0;
    virtual void noFocus() = 0;
    virtual int getMouseListenerId() const = 0;
private:

};

#endif /* IMOUSELISTENER_H */


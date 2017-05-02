/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IDrawable.h
 * Author: user
 *
 * Created on 1 mai 2017, 11:54
 */

#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include "Const.h"
#include "Types.h"
#include <iostream>

class IDrawable {
public:
    virtual ~IDrawable() {} 
    virtual void draw() const = 0;
    virtual bool detectSelection(int x, int y) const = 0;
private:
    
};

#endif /* IDRAWABLE_H */


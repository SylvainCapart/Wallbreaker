/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Colors.h
 * Author: user
 *
 * Created on 3 juillet 2017, 14:59
 */

#ifndef COLORS_H
#define COLORS_H

#include "Types.h"


class Colors {
public:
    Colors();
    virtual ~Colors();
    static const Color st_yellow;  //ok because the colors are singletons
    static const Color st_cobalt;
    static const Color st_red;
    static const Color st_paleRed;
    static const Color st_blueGreen;
    static const Color st_darkBlueGreen;
    static const Color st_paleOrange;
    static const Color st_green;
    static const Color st_black;
    static const Color st_darkCobalt;
    static const Color st_grey;
    static const Color st_white;

private:

};

#endif /* COLORS_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StateContext.h
 * Author: user
 *
 * Created on 2 juillet 2017, 17:13
 */

#ifndef STATECONTEXT_H
#define STATECONTEXT_H

#include "Types.h"

//NB : Not sure this is in the idea of the states pattern but stateContext is used 
// here to transmit information between states during the transition

class StateContext {
public:
    StateContext();
    virtual ~StateContext();
    void setLoadLevelType(T_LOAD_LEVEL loadLevelType);
    T_LOAD_LEVEL getLoadLevelType() const;
    void setMenuType(T_MENU_STATE menuType);
    T_MENU_STATE getMenuType() const;
    void setScore(int32_t score);
    int32_t getScore() const;
    
private:
    T_LOAD_LEVEL m_loadLevelType;
    T_MENU_STATE m_menuType;
    int32_t m_score;

};

#endif /* STATECONTEXT_H */


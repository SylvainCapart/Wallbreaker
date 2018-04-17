/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StateContext.cpp
 * Author: user
 * 
 * Created on 2 juillet 2017, 17:13
 */

#include "StateContext.h"

StateContext::StateContext(): m_loadLevelType(RANDOM_LEVEL), m_menuType(MAIN_MENU) , m_score(0){
   
}


StateContext::~StateContext() {
    
}

void StateContext::setLoadLevelType(T_LOAD_LEVEL loadLevelType) {
    m_loadLevelType = loadLevelType;
}

T_LOAD_LEVEL StateContext::getLoadLevelType() const {
    return m_loadLevelType;
}

void StateContext::setMenuType(T_MENU_STATE menuType) {
    m_menuType = menuType;
}

T_MENU_STATE StateContext::getMenuType() const {
    return m_menuType;
}

void StateContext::setScore(int32_t score) {
    m_score = score;
}

int32_t StateContext::getScore() const {
    return m_score;
}


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EditState.cpp
 * Author: user
 * 
 * Created on 2 juillet 2017, 18:16
 */

#include "EditState.h"
#include "Const.h"
#include "Types.h"
#include "Colors.h"
#include "Fonts.h"
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

EditState::EditState(SDL_Renderer * renderer, Game & game, StateContext & stateContext):
m_renderer(renderer), m_game(game), m_stateContext(stateContext), m_currentBrushState(1),
        m_activatedBrush(false) {
    
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            m_brickMap[i][j] = new Brick(i * BRICK_WIDTH, j * BRICK_HEIGHT , 0, m_renderer);
        }
    }
    m_buttonImage[NO_ACTION] = IMG_LoadTexture(renderer, "img/blue_button_no_action.png");
    m_buttonImage[MOUSE_OVER] = IMG_LoadTexture(renderer, "img/blue_button_mouse_over.png");
    m_buttonImage[PRESSED] = IMG_LoadTexture(renderer, "img/blue_button_pressed.png");
    
    m_exampleBrick = new Brick(5*XU/2, 3*YU/8, 1, m_renderer);
    m_menu = new Menu(renderer);
    
    m_menu->createText(Fonts::st_infoFont, "Selected (SPACE) : ", Colors::st_white, 0.25, 0.33, 2, 0.5);
    m_menu->createButton(B_PLAY_EDITED_LEVEL, m_buttonImage, Fonts::st_infoFont,
            "Save and play", Colors::st_white, 5.5, 10.5, 2, 1);
    m_menu->createButton(B_CLEAR_EDITED_LEVEL, m_buttonImage, Fonts::st_infoFont,
            "Clear", Colors::st_white, 3, 10.5, 1.2, 1);
    m_menu->createButton(B_BACK_FROM_EDIT, m_buttonImage, Fonts::st_arrowFont,
            "B", Colors::st_white, 0.5, 10.8, 1, 0.7);
    
    m_menu->getDrawableList().push_back(m_exampleBrick);
    
}


EditState::~EditState() {
    
    
    SDL_DestroyTexture(m_buttonImage[NO_ACTION]);
    SDL_DestroyTexture(m_buttonImage[MOUSE_OVER]);
    SDL_DestroyTexture(m_buttonImage[PRESSED]);
    delete m_menu;
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            delete m_brickMap[i][j];
        }
    }
}

void EditState::draw() {
    
    SDL_SetRenderDrawColor(m_renderer, Colors::st_cobalt.r, Colors::st_cobalt.g, Colors::st_cobalt.b, SDL_ALPHA_OPAQUE); // setting background color
    SDL_RenderClear(m_renderer); // clearing present renderer
    buildBrickMap();
    m_menu->draw();
    
    
}

void EditState::init() {
    m_mouseX = 0;
    m_mouseY = 0;
    string editedFileName = "lvl/editedLevel.lvl";
    loadFileLevel(m_brickMap, editedFileName);
}

void EditState::onClickDown(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
    for (it = m_menu->getMouseListenerVector().begin(); it != m_menu->getMouseListenerVector().end(); ++it) 
    {
        if ((*it)->detectSelection(m_mouseX, m_mouseY))
            (*it)->onClickDown(evt);
    }
    
    if (detectMouseOnBrickArea())
    {
        m_activatedBrush = true;
        paint();
    }
    
    
    
}

void EditState::onMouseMotion(SDL_Event* evt) {
    vector<IMouseListener*>::const_iterator it;
    for (it = m_menu->getMouseListenerVector().begin(); it != m_menu->getMouseListenerVector().end(); ++it) 
    {
        if ((*it)->detectSelection(m_mouseX, m_mouseY))
            (*it)->onMouseMotion(evt);
    }
    if (detectMouseOnBrickArea() && m_activatedBrush)
        paint();
}

void EditState::onClickUp(SDL_Event* evt) {
    string editedLevelFileName = "lvl/editedLevel.lvl";
    vector<IMouseListener*>::const_iterator it;
    m_activatedBrush = false;
    for (it = m_menu->getMouseListenerVector().begin(); it != m_menu->getMouseListenerVector().end(); ++it)
    {   
        if ((*it)->detectSelection(m_mouseX, m_mouseY))
        {
            (*it)->onClickUp(evt);
            if (B_PLAY_EDITED_LEVEL == (*it)->getMouseListenerId())
            {
                if (!saveFileLevel(m_brickMap, editedLevelFileName))
                    return;
                else 
                {
                    m_stateContext.setLoadLevelType(EDIT_LEVEL);
                    m_game.setCurrentState(GAME_STATE);
                    return; 
                }
            }
            else if (B_BACK_FROM_EDIT == (*it)->getMouseListenerId())
            {
                m_stateContext.setMenuType(LEVEL_CHOICE_MENU);
                m_game.setCurrentState(MENU_STATE);
            }
            else if (B_CLEAR_EDITED_LEVEL == (*it)->getMouseListenerId())
            {
                clearBrickMap();
            }
            // no need for a else case, at worst an unknown button is clicked
        }
    }
    
}


void EditState::onKeyDown(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 1;
}



void EditState::onKeyUp(SDL_Event* evt) {
    m_keys[ evt->key.keysym.sym ] = 0;
    if (SDLK_ESCAPE == evt->key.keysym.sym) {
        m_stateContext.setMenuType(LEVEL_CHOICE_MENU);
        m_game.setCurrentState(MENU_STATE);
    }
    else if (SDLK_SPACE == evt->key.keysym.sym)
    {
        changeBrush();
    }
    
}

void EditState::onEvent(SDL_Event* evt) {
    SDL_GetMouseState(&m_mouseX, &m_mouseY);
    vector<IMouseListener*>::const_iterator it;
    
    for (it = m_menu->getMouseListenerVector().begin(); it != m_menu->getMouseListenerVector().end(); ++it) 
        (*it)->noFocus();
    
    
    switch (evt->type) {
        case SDL_QUIT: onQuit();
        break;
        case SDL_KEYDOWN: onKeyDown(evt);
        break;
        case SDL_KEYUP: onKeyUp(evt);
        break;
        case SDL_MOUSEBUTTONUP: 
            onClickUp(evt);
            break;
        case SDL_MOUSEBUTTONDOWN : 
            onClickDown(evt);
            break;
        case SDL_MOUSEMOTION : 
            onMouseMotion(evt);
            break;
        default:
            
            for (it = m_menu->getMouseListenerVector().begin(); it != m_menu->getMouseListenerVector().end(); ++it) 
            {
                if (!(*it)->detectSelection(m_mouseX, m_mouseY))
                    (*it)->noFocus();
            }
            break;
    }
}



void EditState::onQuit() {
    m_game.stop();
}


void EditState::changeBrush() {
    m_currentBrushState = (m_currentBrushState + 1)%5;
    m_exampleBrick->SetResistance(m_currentBrushState);
}

void EditState::paint() {
    int xIndex = m_mouseX/BRICK_WIDTH;
    int yIndex = m_mouseY/BRICK_HEIGHT;

    if (m_activatedBrush && !((xIndex >= (NB_BRICKS_WIDTH/2 -1) && xIndex <= (NB_BRICKS_WIDTH/2 + NB_BRICKS_WIDTH%2))
            && yIndex >= (NB_BRICKS_HEIGHT/2 - 2) && yIndex <= (NB_BRICKS_HEIGHT/2 + 1 + NB_BRICKS_HEIGHT%2)))
    {
        m_brickMap[xIndex][yIndex]->SetResistance(m_currentBrushState);
    }
}




bool EditState::detectMouseOnBrickArea() {
    return (m_mouseX > (BRICK_AREA_X_ORIGIN_NUMBER*BRICK_WIDTH) && m_mouseX < (BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER)*BRICK_WIDTH
            && m_mouseY > (BRICK_AREA_Y_ORIGIN_NUMBER*BRICK_HEIGHT) && m_mouseY < (BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER)*BRICK_HEIGHT);
} 

void EditState::buildBrickMap() {
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            m_brickMap[i][j]->draw();
        }
    }
}

/* Function : loadFileLevel
 * 
 * Input : int brickMap[BRICK_AREA_X_NUMBER][BRICK_AREA_Y_NUMBER] : the 2D table
 * to fill with numbers following the characters in the loaded file.
 * 
 * Output : true on success or 0false on failure
 * 
 * Goal : fill the brickMap with correct data
 */

bool EditState::loadFileLevel(Brick * brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string& fileName) {
    string fileLign;
    stringstream buffer;
    ifstream file(fileName.c_str());
    if (file.good())
    {
        buffer << file.rdbuf();
    }
    else
    {
        cerr << "ERROR : Level file not found. Try regenerating Makelist" << endl;
        return false;
    }
    fileLign = buffer.str();
    
    
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            brickMap[i][j]->SetResistance( (fileLign[i * NB_BRICKS_WIDTH + j] - '0') % 48);
        }
    }
    file.close();
    return true;
}

bool EditState::saveFileLevel(Brick* brickMap[NB_BRICKS_WIDTH][NB_BRICKS_HEIGHT], std::string& fileName) {
    string input = "";
    ofstream out;
    out.open(fileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    if (!out.is_open())
        return false;
    
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            input.append(to_string(0));
        }
    }
    
    for (int i = BRICK_AREA_X_ORIGIN_NUMBER; i < BRICK_AREA_X_ORIGIN_NUMBER + BRICK_AREA_X_NUMBER; ++i) {
        for (int j = BRICK_AREA_Y_ORIGIN_NUMBER; j < BRICK_AREA_Y_ORIGIN_NUMBER + BRICK_AREA_Y_NUMBER; ++j) {
            input.replace((i * NB_BRICKS_WIDTH) + j, 1, to_string(brickMap[i][j]->GetResistance())); 
        }
    }
    out << input;
    out.close();
    return true;
}

string EditState::to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

/* Function : clearBrickMap
 * 
 * Goal : set m_brickmap resistances to 0
 */
void EditState::clearBrickMap() {
    
    for (int i = 0; i < NB_BRICKS_WIDTH; ++i) {
        for (int j = 0; j < NB_BRICKS_HEIGHT; ++j) {
            m_brickMap[i][j]->SetResistance(0);
        }
    }
}


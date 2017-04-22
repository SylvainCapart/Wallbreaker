/* 
 * File:   Menu.cpp
 * Author: Sylvain
 * 
 * Created on 9 avril 2017, 11:57
 */

#include "Menu.h"

using namespace std;

Menu::Menu(SDL_Renderer * renderer) {

    titleFont = TTF_OpenFont("font/play the game.ttf", 64);
    if (!titleFont) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
    }

    infoFont = TTF_OpenFont("font/LinLibertine_R.ttf", 64);
    if (!infoFont) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
    }

    menuTexture = IMG_LoadTexture(renderer, "img/space1.jpg");
    if (menuTexture == NULL)
        cout << "Error charging menu graphics" << endl;

    buttonImage[NO_ACTION] = IMG_LoadTexture(renderer, "img/button_no_action.png");
    buttonImage[MOUSE_OVER] = IMG_LoadTexture(renderer, "img/button_mouse_over.png");
    buttonImage[PRESSED] = IMG_LoadTexture(renderer, "img/button_pressed.png");

    this->renderer = renderer;
    buttonInterval = 30;
    SDL_Color white = {255, 255, 255};

    SDL_Rect startRect = {100, 220, 300, 80};
    SDL_Rect quitRect = {100, 350, 300, 80};
    Button startButton(startRect, renderer, "Start", white, *infoFont, buttonImage);
    
    Button quitButton(quitRect, renderer, "Quit", white, *infoFont, buttonImage);


    buttonVector.push_back(startButton);
    buttonVector.push_back(quitButton);







}

Menu::~Menu() {

    for (int i = 0; i < 3; ++i)
        SDL_DestroyTexture(buttonImage[i]);
    
        TTF_CloseFont(infoFont);
    TTF_CloseFont(titleFont);

}

void Menu::init() {



    //SDL_RenderPresent(renderer);

}

void Menu::update() {

    SDL_RenderCopy(renderer, this->menuTexture, NULL, NULL);
    SDL_Color white = {255, 255, 255};


    const static std::string wallbreakerStr = "Wallbreaker";
    const static std::string txt2 = "Player 1 : QZSD";
    const static std::string txt3 = "Player 2 : UP DOWN";
    const static std::string txt4 = "LEFT RIGHT";
    const static std::string txt5 = "SPACE : Launch ball";
    const static std::string txt6 = "and middle racket";
    const static std::string txt7 = "ESCAPE : Quit";
    
    drawTextBlended(titleFont, wallbreakerStr, white, 100, 70, 600, 100);
    drawTextBlended(infoFont, txt2, white, 450, 200, 300, 60);
    drawTextBlended(infoFont, txt3, white, 450, 250, 340, 60);
    drawTextBlended(infoFont, txt4, white, 450, 310, 170, 60);
    drawTextBlended(infoFont, txt5, white, 450, 360, 300, 60);
    drawTextBlended(infoFont, txt6, white, 450, 410, 300, 60);
    drawTextBlended(infoFont, txt7, white, 450, 460, 300, 60);



    vector<Button>::iterator it;
    for (it = buttonVector.begin(); it != buttonVector.end(); ++it) {

        (*it).draw(renderer);

    }


    SDL_RenderPresent(renderer);

}

void Menu::drawTextBlended(TTF_Font * font, const string & text, const SDL_Color & color, int x, int y, int w, int h) {
    SDL_Surface* surfaceMessage = NULL;
    surfaceMessage = TTF_RenderText_Blended(font, text.c_str(), color);

    SDL_Texture* message = NULL;
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect = {x, y, w, h};

    SDL_RenderCopy(renderer, message, NULL, &messageRect);
    SDL_FreeSurface(surfaceMessage);
}



#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gui/Label.h"
#include "gui/Button.h"

#define MAINMENUSTATE_ID 0

struct MainMenuState {
    Label* title;
    Button* play_button;    
};

typedef struct MainMenuState MainMenuState;

MainMenuState *MainMenuState_Create();
void MainMenuState_Draw(MainMenuState* state, SDL_Renderer* renderer);
void MainMenuState_HandleEvent(MainMenuState *state, SDL_Event *event);
void MainMenuState_Update(MainMenuState* state);
void MainMenuState_Free(MainMenuState* state);
void OnPlayButton_Click();

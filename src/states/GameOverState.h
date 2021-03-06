#pragma once
#include "../gui/Label.h"
#include "../gui/Button.h"
#include "../gui/Fonts.h"

#define GAMEOVERSTATE_ID 2

typedef struct GameOverState {
    Label* winning_player_label;
    Button* go_back_button;
} GameOverState;

GameOverState* GameOverState_Create();
void GameOverState_HandleEvent(GameOverState *state, SDL_Event *event);
void GameOverState_Update(GameOverState* state);
void GameOverState_Draw(GameOverState* state, SDL_Renderer *renderer);
void GameOverState_Free(GameOverState* state);
void GoBackButton_OnClick();

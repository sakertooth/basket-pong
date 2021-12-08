#pragma once
#include <SDL2/SDL.h>
#include "gui/Fonts.h"
#include "states/MainMenuState.h"
#include "states/PlayState.h"
#include "states/GameOverState.h"

#define GAME_TITLE "Basket Pong"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_bool running;
    Fonts* fonts;
    PlayState* play_state;
    MainMenuState* mainmenu_state;
    GameOverState* gameover_state;
    int current_state;
};

typedef struct Game Game;

Game* Game_Init();
void Game_Draw(Game *game);
void Game_HandleEvent(Game *game, SDL_Event *event);
void Game_Update(Game *game, float delta_time);
void Game_Exit(Game *game);

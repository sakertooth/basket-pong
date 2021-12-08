#include "Game.h"

Game* Game_Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        return NULL;
    }

    Game *game = malloc(sizeof(Game));
    if (game == NULL) {
        return NULL;
    }

    game->window = SDL_CreateWindow("Basket Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (game->window == NULL) {
        goto error1;
    }
    
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        goto error2;
    }

    game->fonts = Fonts_Init();
    if (game->fonts == NULL) {
        goto error3;
    }
    
    game->running = SDL_TRUE;
    game->mainmenu_state = NULL;
    game->play_state = NULL;
    game->current_state = -1;

    return game;

error3:
    SDL_DestroyRenderer(game->renderer);
error2:
    SDL_DestroyWindow(game->window);
error1:
    free(game);
    return NULL;
}


void Game_Draw() {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
    SDL_RenderClear(game->renderer);

    switch (game->current_state) {
        case MAINMENUSTATE_ID:
            MainMenuState_Draw(game->mainmenu_state, game->renderer);
            break;
        case PLAYSTATE_ID:
            PlayState_Draw(game->play_state);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Draw(game->gameover_state, game->renderer);
            break;
        default:
            break;
    }
    
    SDL_RenderPresent(game->renderer);
}

void Game_HandleEvent(SDL_Event *event) {
    switch (game->current_state) {
        case MAINMENUSTATE_ID:
            MainMenuState_HandleEvent(game->mainmenu_state, event);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_HandleEvent(game->gameover_state, event);
            break;
        default:
            break;
    }
}

void Game_Update(float delta_time) {
    switch (game->current_state) {
        case MAINMENUSTATE_ID:
            MainMenuState_Update(game->mainmenu_state);
            break;
        case PLAYSTATE_ID:
            PlayState_Update(game->play_state, delta_time);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Update(game->gameover_state);
            break;
        default:
            break;
    }
}


void Game_Exit() {
    game->running = SDL_FALSE;
    MainMenuState_Free(game->mainmenu_state);
    PlayState_Free(game->play_state);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    Fonts_Free(game->fonts);
    SDL_Quit();
    TTF_Quit();
    free(game);
}

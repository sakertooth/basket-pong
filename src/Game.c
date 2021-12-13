#include "Game.h"

Game* Game_Create() {
    Game *game = malloc(sizeof(Game));
    if (game == NULL) {
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        goto error1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        goto error2;
    }

    Fonts *fonts = Fonts_Init();
    if (fonts == NULL) {
        goto error3;
    }

    Audio *audio = Audio_Create();
    if (audio == NULL) {
        goto error4;
    }
    
    game->running = SDL_TRUE;
    game->current_state_id = -1;

    game->mainmenu_state = NULL;
    game->play_state = NULL;
    game->gameover_state = NULL;
 
    game->window = window;
    game->renderer = renderer;
    game->fonts = fonts;
    game->audio = audio;
    return game;

error4:
    Fonts_Free(fonts);
error3:
    SDL_DestroyRenderer(game->renderer);
error2:
    SDL_DestroyWindow(game->window);
error1:
    free(game);
    return NULL;
}

void Game_Draw() {
    if (!basket_pong_game->running) {
        return;
    }

    SDL_SetRenderDrawColor(basket_pong_game->renderer, 0, 0, 0, 0);
    SDL_RenderClear(basket_pong_game->renderer);

    switch (basket_pong_game->current_state_id) {
        case MAINMENUSTATE_ID:
            MainMenuState_Draw(basket_pong_game->mainmenu_state, basket_pong_game->renderer);
            break;
        case PLAYSTATE_ID:
            PlayState_Draw(basket_pong_game->play_state, basket_pong_game->renderer);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Draw(basket_pong_game->gameover_state, basket_pong_game->renderer);
            break;
        default:
            break;
    }
    
    SDL_RenderPresent(basket_pong_game->renderer);
}

void Game_HandleEvent() {
    if (!basket_pong_game->running) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            basket_pong_game->running = SDL_FALSE;
            return;
        }

        switch (basket_pong_game->current_state_id) {
            case MAINMENUSTATE_ID:
                MainMenuState_HandleEvent(basket_pong_game->mainmenu_state, &event);
                break;
            case GAMEOVERSTATE_ID:
                GameOverState_HandleEvent(basket_pong_game->gameover_state, &event);
                break;
            default:
                break;
        }
    }
}

void Game_Update(float delta_time) {
    if (!basket_pong_game->running) {
        return;
    }

    switch (basket_pong_game->current_state_id) {
        case MAINMENUSTATE_ID:
            MainMenuState_Update(basket_pong_game->mainmenu_state);
            break;
        case PLAYSTATE_ID:
            PlayState_Update(basket_pong_game->play_state, delta_time);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Update(basket_pong_game->gameover_state);
            break;
        default:
            break;
    }
}

void Game_SwitchState(int state_id) {
    switch (basket_pong_game->current_state_id) {
        case MAINMENUSTATE_ID:
            MainMenuState_Free(basket_pong_game->mainmenu_state);
            basket_pong_game->mainmenu_state = NULL;
            break;
        case PLAYSTATE_ID:
            PlayState_Free(basket_pong_game->play_state);
            basket_pong_game->play_state = NULL;
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Free(basket_pong_game->gameover_state);
            basket_pong_game->gameover_state = NULL;
            break;
    }

    switch (state_id) {
        case MAINMENUSTATE_ID:
            basket_pong_game->mainmenu_state = MainMenuState_Create();
            basket_pong_game->current_state_id = MAINMENUSTATE_ID;
            break;
        case PLAYSTATE_ID:
            basket_pong_game->play_state = PlayState_Create();
            basket_pong_game->current_state_id = PLAYSTATE_ID;
            break;
        case GAMEOVERSTATE_ID:
            basket_pong_game->gameover_state = GameOverState_Create();
            basket_pong_game->current_state_id = GAMEOVERSTATE_ID;
            break;
        default:
            break;
    }
}

void Game_Exit() {
    MainMenuState_Free(basket_pong_game->mainmenu_state);
    PlayState_Free(basket_pong_game->play_state);
    GameOverState_Free(basket_pong_game->gameover_state);    
    SDL_DestroyRenderer(basket_pong_game->renderer);
    SDL_DestroyWindow(basket_pong_game->window);
    Fonts_Free(basket_pong_game->fonts);
    free(basket_pong_game);
}

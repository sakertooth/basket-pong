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
    
    game->running = SDL_TRUE;
    game->current_state = -1;

    game->mainmenu_state = MainMenuState_Create(fonts, renderer, &game->current_state);
    game->play_state = PlayState_Create(fonts, renderer);
    game->gameover_state = GameOverState_Create(fonts, renderer, &game->current_state);
 
    game->window = window;
    game->renderer = renderer;
    game->fonts = fonts;
    return game;

error3:
    SDL_DestroyRenderer(game->renderer);
error2:
    SDL_DestroyWindow(game->window);
error1:
    free(game);
    return NULL;
}

void Game_Draw(Game *game) {
    if (!game->running) {
        return;
    }

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
    SDL_RenderClear(game->renderer);

    switch (game->current_state) {
        case MAINMENUSTATE_ID:
            MainMenuState_Draw(game->mainmenu_state, game->renderer);
            break;
        case PLAYSTATE_ID:
            PlayState_Draw(game->play_state, game->renderer);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Draw(game->gameover_state, game->renderer);
            break;
        default:
            break;
    }
    
    SDL_RenderPresent(game->renderer);
}

void Game_HandleEvent(Game *game) {
    if (!game->running) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->running = SDL_FALSE;
            return;
        }

        switch (game->current_state) {
            case MAINMENUSTATE_ID:
                MainMenuState_HandleEvent(game->mainmenu_state, &event);
                break;
            case GAMEOVERSTATE_ID:
                GameOverState_HandleEvent(game->gameover_state, &event);
                break;
            default:
                break;
        }
    }
}

void Game_Update(Game *game, float delta_time) {
    if (!game->running) {
        return;
    }

    switch (game->current_state) {
        case MAINMENUSTATE_ID:
            MainMenuState_Update(game->mainmenu_state);
            break;
        case PLAYSTATE_ID:
            PlayState_Update(game->play_state, &game->current_state, delta_time);
            break;
        case GAMEOVERSTATE_ID:
            GameOverState_Update(game->gameover_state);
            break;
        default:
            break;
    }
}

void Game_Exit(Game *game) {
    MainMenuState_Free(game->mainmenu_state);
    PlayState_Free(game->play_state);
    GameOverState_Free(game->gameover_state);    
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    Fonts_Free(game->fonts);
    free(game);
}

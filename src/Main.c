#include "Game.h"

Game *basket_pong_game = NULL;

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initalize SDL. %s\n.", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize TTF. %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Failed to initialize SDL2_Mixer. %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }

    basket_pong_game = Game_Create();
    if (basket_pong_game == NULL) {
        fprintf(stderr, "Failed to start game.");
        return EXIT_FAILURE;
    }

    Game_SwitchState(MAINMENUSTATE_ID);

    Uint32 last = SDL_GetTicks();
    while (basket_pong_game->running) {

        Uint32 now = SDL_GetTicks();
        float delta_time = (float) (now - last) / 1000;
        last = now;

        Game_HandleEvent();
        Game_Update(delta_time);
        Game_Draw();
    }

    Game_Exit();
    SDL_Quit();
    TTF_Quit();
}

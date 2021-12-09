#include "Game.h"

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initalize SDL. %s\n.", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize TTF. %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    Game* game = Game_Create();
    if (game == NULL) {
        fprintf(stderr, "Failed to start game.");
        return EXIT_FAILURE;
    }

    game->current_state = MAINMENUSTATE_ID;

    Uint32 last = SDL_GetTicks();
    while (game->running) {

        Uint32 now = SDL_GetTicks();
        float delta_time = (float) (now - last) / 1000;
        last = now;

        Game_HandleEvent(game);
        Game_Update(game, delta_time);
        Game_Draw(game);
    }

    Game_Exit(game);
    SDL_Quit();
    TTF_Quit();
}

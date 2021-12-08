#include "Game.h"
#include <time.h>

int main() {
    Game *game = Game_Init();
    if (game == NULL) {
        fprintf(stderr, "Failed to start game: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    game->mainmenu_state = MainMenuState_Create(game->renderer);
    game->current_state = MAINMENUSTATE_ID;

    while (game->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game->running = SDL_FALSE;
                    Game_Exit(game);
                    return EXIT_SUCCESS;
                default:
                    break;
            }

            Game_HandleEvent(game, &event);
        }

        Uint32 start = SDL_GetTicks();
        Game_Draw(game);
        Uint32 end = SDL_GetTicks();
        Game_Update(game, (float) (end - start) / 1000);
    }
}

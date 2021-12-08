#include "GameOverState.h"
#include "Game.h"
#include <stdlib.h>

GameOverState* GameOverState_Create(char* winning_player, SDL_Renderer* renderer) {
    GameOverState* state = malloc(sizeof(GameOverState));
    if (state == NULL) {
        return NULL;
    }

    char *win_str = malloc(strlen(winning_player) + 10 /* has won!\0*/);
    if (win_str == NULL) {
        goto cleanup1;
    }

    sprintf(win_str, "%s has won!", winning_player);
 
    SDL_Color color_white = {255, 255, 255, 0};
    state->winning_player_label = Label_Create(win_str, 0, 0, game->fonts->large_font, game->renderer, color_white);
    state->winning_player_label->rect.x = SCREEN_WIDTH / 2 - state->winning_player_label->rect.w / 2;
    state->winning_player_label->rect.y = SCREEN_HEIGHT / 2 - state->winning_player_label->rect.h / 2 - 100;
    
    if (state->winning_player_label == NULL) {
        goto cleanup2;
    }

    SDL_Color color_black = {0, 0, 0, 0};
    state->go_back_button = Button_Create("Go back", 0, 0, GoBackButton_OnClick, color_black, color_white, renderer);
    Button_SetPosition(state->go_back_button, SCREEN_WIDTH / 2 - state->go_back_button->rect.w / 2, SCREEN_HEIGHT / 2 - state->go_back_button->rect.h / 2);

    if (state->go_back_button == NULL) {
        goto cleanup3;
    }

    return state;

cleanup3:
    Label_Free(state->winning_player_label);
cleanup2:
    free(win_str);
cleanup1:
    free(state);
    return NULL;
}

void GameOverState_HandleEvent(GameOverState *state, SDL_Event *event) {
    Button_HandleEvent(state->go_back_button, event);
}

void GameOverState_Update(GameOverState* state) {
    Button_Update(state->go_back_button);
}

void GameOverState_Draw(GameOverState* state, SDL_Renderer *renderer) {
    Label_Draw(state->winning_player_label, renderer);
    Button_Draw(state->go_back_button, renderer);
}

void GameOverState_Free(GameOverState* state) {
    if (state == NULL) {
        return;
    }

    Label_Free(state->winning_player_label);
    Button_Free(state->go_back_button);
}

void GoBackButton_OnClick() {
    GameOverState_Free(game->gameover_state);
    game->gameover_state = NULL;
    game->mainmenu_state = MainMenuState_Create();
    game->current_state = MAINMENUSTATE_ID;
}

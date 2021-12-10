#include "GameOverState.h"
#include "Game.h"
#include <stdlib.h>

GameOverState* GameOverState_Create() {
    GameOverState* state = malloc(sizeof(GameOverState));
    if (state == NULL) {
        return NULL;
    }
 
    SDL_Color color_white = {255, 255, 255, 0};
    SDL_Color color_black = {0, 0, 0, 0};

    Label *winning_player_label = Label_Create(".", 0, 0, basket_pong_game->fonts->large_font, &color_white, basket_pong_game->renderer);
    if (winning_player_label == NULL) {
        goto cleanup1;
    }
    
    Button* go_back_button = Button_Create("Go back", 0, 0, GoBackButton_OnClick, basket_pong_game->fonts->large_font, &color_black, &color_white, basket_pong_game->renderer);
    if (go_back_button == NULL) {
        goto cleanup2;
    }

    state->go_back_button = go_back_button;
    Button_SetPosition(state->go_back_button, SCREEN_WIDTH / 2 - state->go_back_button->rect.w / 2, SCREEN_HEIGHT / 2 - state->go_back_button->rect.h / 2);

    state->winning_player_label = winning_player_label;
    state->winning_player_label->rect.x = state->go_back_button->rect.x + (state->go_back_button->rect.w - state->winning_player_label->rect.w) / 2;
    state->winning_player_label->rect.y = SCREEN_HEIGHT / 2 - state->winning_player_label->rect.h / 2 - 100;


    return state;

cleanup2:
    Label_Free(state->winning_player_label);
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
    Game_SwitchScene(MAINMENUSTATE_ID);
}

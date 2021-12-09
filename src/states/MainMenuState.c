#include <stdio.h>
#include "MainMenuState.h"
#include "Game.h"

MainMenuState *MainMenuState_Create(Fonts* fonts, SDL_Renderer* renderer, int* current_state) {
    MainMenuState* mainmenu_state = malloc(sizeof(MainMenuState));
    if (mainmenu_state == NULL) {
        return NULL;
    }

    SDL_Color color_white = {255, 255, 255, 0};
    SDL_Color color_black = {0, 0, 0, 0};

    mainmenu_state->title = Label_Create("Basket Pong", 0, 50, fonts->title_font, &color_white, renderer);
    if (mainmenu_state->title == NULL) {
        goto cleanup1;
    }

    mainmenu_state->title->rect.x = SCREEN_WIDTH / 2 - mainmenu_state->title->rect.w / 2;
    mainmenu_state->play_button = Button_Create("Play", 0, 0, OnPlayButton_Click, (void*) current_state, fonts->title_font, &color_black, &color_white, renderer);
    if (mainmenu_state->play_button == NULL) {
        goto cleanup2;
    }
    
    int play_button_x = (SCREEN_WIDTH - mainmenu_state->play_button->rect.w) / 2;
    int play_button_y = (SCREEN_HEIGHT - mainmenu_state->play_button->rect.h) / 2;
    Button_SetPosition(mainmenu_state->play_button, play_button_x, play_button_y);
    
    return mainmenu_state;
cleanup2:
    Label_Free(mainmenu_state->title);
cleanup1:
    free(mainmenu_state);
    return NULL;
}

void MainMenuState_Draw(MainMenuState *state, SDL_Renderer* renderer) {
    Label_Draw(state->title, renderer);
    Button_Draw(state->play_button, renderer);
}

void MainMenuState_HandleEvent(MainMenuState *state, SDL_Event *event) {
    Button_HandleEvent(state->play_button, event);    
}

void MainMenuState_Update(MainMenuState *state) {
    Button_Update(state->play_button);
}

void MainMenuState_Free(MainMenuState *state) {
    if (state == NULL) {
        return;
    }

    Label_Free(state->title);
    Button_Free(state->play_button);
    free(state);
}

void OnPlayButton_Click(void *arg) {
    int *current_state = (int*) arg;
    *current_state = PLAYSTATE_ID;
}

#pragma once
#include "objects/Player.h"
#include "objects/Ball.h"
#include "objects/Net.h"
#include "gui/Label.h"
#include "gui/Fonts.h"
#include "GameOverState.h"

#define PLAYSTATE_ID 1
#define SCREEN_FLOOR SCREEN_HEIGHT - 5
#define WINNING_SCORE 5

typedef struct PlayState {
    Player* player_one;
    Player* player_two;
    Net* player_one_net;
    Net* player_two_net;
    Label* player_one_score_label;
    Label* player_two_score_label;
    Label* player_one_score_indicator;
    Label* player_two_score_indicator;
    Label* player_one_indicator;
    Label* player_two_indicator;
    Label* scoreboard_separator;
    SDL_Renderer* active_renderer;
    int player_one_score;
    int player_two_score;
    Ball* ball;
} PlayState;

PlayState *PlayState_Create(Fonts* fonts, SDL_Renderer *renderer);
void PlayState_Draw(PlayState *state, SDL_Renderer *renderer);
void PlayState_Update(PlayState *state, GameOverState *game_over_state, int *current_state, float delta_time);
void PlayState_OnScore(int score, Ball* ball, Label *label);
void PlayState_Reset(PlayState *state);
void PlayState_Free(PlayState *state);
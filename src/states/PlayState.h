#pragma once
#include "objects/Player.h"
#include "objects/Ball.h"
#include "objects/Net.h"
#include "gui/Label.h"

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
    int player_one_score;
    int player_two_score;
    Ball* ball;
} PlayState;

PlayState *PlayState_Create();
void PlayState_Draw(PlayState *state);
void PlayState_Update(PlayState *state, float delta_time);
void PlayState_OnScore(int score, Ball* ball, Label *label);
void PlayState_Free(PlayState *state);
void PlayState_EndGame(char *winning_player);

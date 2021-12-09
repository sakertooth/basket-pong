#include "PlayState.h"
#include "Game.h"

PlayState *PlayState_Create(Fonts* fonts, SDL_Renderer *renderer) {
    PlayState *play_state = malloc(sizeof(PlayState));
    if (play_state == NULL) {
        return NULL;
    }

    play_state->player_one = Player_Create(50, SCREEN_FLOOR - PLAYER_HEIGHT, SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_W, SDL_SCANCODE_SPACE);
    play_state->player_two = Player_Create(SCREEN_WIDTH - PLAYER_WIDTH - 50, SCREEN_FLOOR - PLAYER_HEIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_SPACE);
    play_state->ball = Ball_Create(SCREEN_WIDTH / 2 - BALL_SIZE, SCREEN_HEIGHT - BALL_SIZE - 5);

    play_state->player_one_net = Net_Create(0, SCREEN_HEIGHT / 2 - NET_BOARD_HEIGHT - 60, SDL_FALSE);
    play_state->player_two_net = Net_Create(SCREEN_WIDTH - NET_BOARD_WIDTH, SCREEN_HEIGHT / 2 - NET_BOARD_HEIGHT - 60, SDL_TRUE);

    SDL_Color color_white = {255, 255, 255, 0};

    play_state->player_one_score_label = Label_Create("0", 0, 0, fonts->large_font, &color_white, renderer);
    play_state->player_one_score_label->rect.x = SCREEN_WIDTH / 2 - play_state->player_one_score_label->rect.x - 100;
    play_state->player_one_score_label->rect.y = SCREEN_HEIGHT / 2 - play_state->player_one_score_label->rect.y - 200;

    play_state->player_two_score_label = Label_Create("0", 0, 0, fonts->large_font, &color_white, renderer);
    play_state->player_two_score_label->rect.x = SCREEN_WIDTH / 2 - play_state->player_two_score_label->rect.x + 100;
    play_state->player_two_score_label->rect.y = SCREEN_HEIGHT / 2 - play_state->player_two_score_label->rect.y - 200;

    play_state->player_one_score_indicator = Label_Create("Player 1", 0, 0, fonts->medium_font, &color_white, renderer);
    play_state->player_one_score_indicator->rect.x = play_state->player_one_score_label->rect.x + (play_state->player_one_score_label->rect.w - play_state->player_one_score_indicator->rect.w) / 2;
    play_state->player_one_score_indicator->rect.y = play_state->player_one_score_label->rect.y - play_state->player_one_score_label->rect.h / 2 - 10;

    play_state->player_two_score_indicator = Label_Create("Player 2", 0, 0, fonts->medium_font, &color_white, renderer);
    play_state->player_two_score_indicator->rect.x = play_state->player_two_score_label->rect.x + (play_state->player_two_score_label->rect.w - play_state->player_two_score_indicator->rect.w) / 2;
    play_state->player_two_score_indicator->rect.y = play_state->player_two_score_label->rect.y - play_state->player_two_score_label->rect.h / 2 - 10;
    
    int scoreboard_separator_x = play_state->player_one_score_label->rect.x + (play_state->player_two_score_label->rect.x - play_state->player_one_score_label->rect.x) / 2;
    int scoreboard_separator_y = play_state->player_one_score_label->rect.y + (play_state->player_one_score_label->rect.h / 2) - 50;
    play_state->scoreboard_separator = Label_Create("|", scoreboard_separator_x, scoreboard_separator_y, fonts->small_font, &color_white, renderer);
    play_state->scoreboard_separator->rect.h = 100;

    play_state->player_one_indicator = Label_Create("1", play_state->player_one->pos_x, play_state->player_one->pos_y, fonts->medium_font, &color_white, renderer);
    play_state->player_two_indicator = Label_Create("2", play_state->player_two->pos_x, play_state->player_two->pos_y, fonts->medium_font, &color_white, renderer);

    play_state->player_one_score = 0;
    play_state->player_two_score = 0;
    play_state->active_renderer = renderer;
    return play_state;
}

void PlayState_Draw(PlayState *state, SDL_Renderer *renderer) {
    Player_Draw(state->player_one, renderer);
    Player_Draw(state->player_two, renderer);
    Ball_Draw(state->ball, renderer);
    Net_Draw(state->player_one_net, renderer);
    Net_Draw(state->player_two_net, renderer);
    Label_Draw(state->player_one_score_label, renderer);
    Label_Draw(state->player_two_score_label, renderer);
    Label_Draw(state->player_one_indicator, renderer);
    Label_Draw(state->player_two_indicator, renderer);
    Label_Draw(state->player_one_score_indicator, renderer);
    Label_Draw(state->player_two_score_indicator, renderer);
    Label_Draw(state->scoreboard_separator, renderer);
}

void PlayState_Update(PlayState *state, GameOverState *game_over_state, int *current_state, float delta_time) {
    Player_Update(state->player_one, state->ball, delta_time);
    Player_Update(state->player_two, state->ball, delta_time);
    Ball_Update(state->ball, delta_time);
    Net_Update(state->player_one_net, state->ball);
    Net_Update(state->player_two_net, state->ball);

    state->player_one_indicator->rect.x = state->player_one->pos_x - PLAYER_WIDTH / 4;
    state->player_one_indicator->rect.y = state->player_one->shoot_meter_rect.y - state->player_one->shoot_meter_rect.h - 15;

    state->player_two_indicator->rect.x = state->player_two->pos_x - PLAYER_WIDTH / 4;
    state->player_two_indicator->rect.y = state->player_two->shoot_meter_rect.y - state->player_two->shoot_meter_rect.h - 15;

    if (Net_BallInHoop(state->player_one_net, state->ball)) {
        PlayState_OnScore(++state->player_two_score, state->ball, state->player_two_score_label);
    }

    if (Net_BallInHoop(state->player_two_net, state->ball)) {
        PlayState_OnScore(++state->player_one_score, state->ball, state->player_one_score_label);
    }

    if (state->player_one_score == WINNING_SCORE) {
        *current_state = GAMEOVERSTATE_ID;
        PlayState_Reset(state);
        GameOverState_SetText(game_over_state, "Player One Wins!");
    }
    else if (state->player_two_score == WINNING_SCORE) {
        *current_state = GAMEOVERSTATE_ID;
        PlayState_Reset(state);
        GameOverState_SetText(game_over_state, "Player Two Wins!");
    }
}

void PlayState_OnScore(int score, Ball* ball, Label *label) {
    ball->pos_y += BALL_SIZE;
    ball->vel_y /= 2;

    int ndigits = snprintf(NULL, 0, "%d", score);
    char *score_buf = malloc(ndigits + 1);
    sprintf(score_buf, "%d", score);

    SDL_Color color_white = {255, 255, 255, 0};
    Label_SetText(label, score_buf);
    Label_SetColor(label, &color_white);  

    free(score_buf);  
}

void PlayState_Reset(PlayState *state) {
    state->player_one_score = 0;
    state->player_two_score = 0;
    state->player_one->has_ball = SDL_FALSE;
    state->player_one->is_shooting = SDL_FALSE;
    state->player_one->jumping = SDL_FALSE;
    state->player_one->facing_right = SDL_TRUE;
    state->player_one->vel_y = 0;

    Label_SetText(state->player_one_score_label, "0");
    Label_SetText(state->player_two_score_label, "0");

    state->player_one->pos_x = 50;
    state->player_one->pos_y = SCREEN_FLOOR - PLAYER_HEIGHT;

    state->player_two->pos_x = SCREEN_WIDTH - PLAYER_WIDTH - 50;
    state->player_two->pos_y  = SCREEN_FLOOR - PLAYER_HEIGHT;
    state->player_two->has_ball = SDL_FALSE;
    state->player_two->is_shooting = SDL_FALSE;
    state->player_two->jumping = SDL_FALSE;
    state->player_two->facing_right = SDL_FALSE;
    state->player_two->vel_y = 0;

    state->ball->pos_x = SCREEN_WIDTH / 2 - BALL_SIZE;
    state->ball->pos_y = SCREEN_HEIGHT - BALL_SIZE - 5;
    state->ball->vel_x = 0;
    state->ball->vel_y = 0;

}

void PlayState_Free(PlayState *state) {
    if (state == NULL) {
        return;
    }
    
    free(state->player_one_net);
    free(state->player_two_net);
    free(state->player_one);
    free(state->player_two);
    free(state->ball);
    Label_Free(state->player_one_score_label);
    Label_Free(state->player_two_score_label);
    Label_Free(state->player_one_score_indicator);
    Label_Free(state->player_two_score_indicator);
    Label_Free(state->scoreboard_separator);
    free(state);
}

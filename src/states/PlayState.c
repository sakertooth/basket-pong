#include "PlayState.h"
#include "Game.h"

PlayState *PlayState_Create() {
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

    play_state->player_one_score_label = Label_Create("0", 0, 0, basket_pong_game->fonts->large_font, &color_white, basket_pong_game->renderer);
    play_state->player_one_score_label->rect.x = SCREEN_WIDTH / 2 - play_state->player_one_score_label->rect.x - 100;
    play_state->player_one_score_label->rect.y = SCREEN_HEIGHT / 2 - play_state->player_one_score_label->rect.y - 200;

    play_state->player_two_score_label = Label_Create("0", 0, 0, basket_pong_game->fonts->large_font, &color_white, basket_pong_game->renderer);
    play_state->player_two_score_label->rect.x = SCREEN_WIDTH / 2 - play_state->player_two_score_label->rect.x + 100;
    play_state->player_two_score_label->rect.y = SCREEN_HEIGHT / 2 - play_state->player_two_score_label->rect.y - 200;

    play_state->player_one_score_indicator = Label_Create("Player 1", 0, 0, basket_pong_game->fonts->medium_font, &color_white, basket_pong_game->renderer);
    play_state->player_one_score_indicator->rect.x = play_state->player_one_score_label->rect.x + (play_state->player_one_score_label->rect.w - play_state->player_one_score_indicator->rect.w) / 2;
    play_state->player_one_score_indicator->rect.y = play_state->player_one_score_label->rect.y - play_state->player_one_score_label->rect.h / 2 - 10;

    play_state->player_two_score_indicator = Label_Create("Player 2", 0, 0, basket_pong_game->fonts->medium_font, &color_white, basket_pong_game->renderer);
    play_state->player_two_score_indicator->rect.x = play_state->player_two_score_label->rect.x + (play_state->player_two_score_label->rect.w - play_state->player_two_score_indicator->rect.w) / 2;
    play_state->player_two_score_indicator->rect.y = play_state->player_two_score_label->rect.y - play_state->player_two_score_label->rect.h / 2 - 10;
    
    int scoreboard_separator_x = play_state->player_one_score_label->rect.x + (play_state->player_two_score_label->rect.x - play_state->player_one_score_label->rect.x) / 2;
    int scoreboard_separator_y = play_state->player_one_score_label->rect.y + (play_state->player_one_score_label->rect.h / 2) - 50;
    play_state->scoreboard_separator = Label_Create("|", scoreboard_separator_x, scoreboard_separator_y, basket_pong_game->fonts->small_font, &color_white, basket_pong_game->renderer);
    play_state->scoreboard_separator->rect.h = 100;

    play_state->player_one_indicator = Label_Create("1", play_state->player_one->pos_x, play_state->player_one->pos_y, basket_pong_game->fonts->medium_font, &color_white, basket_pong_game->renderer);
    play_state->player_two_indicator = Label_Create("2", play_state->player_two->pos_x, play_state->player_two->pos_y, basket_pong_game->fonts->medium_font, &color_white, basket_pong_game->renderer);

    play_state->player_one_score = 0;
    play_state->player_two_score = 0;
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

void PlayState_Update(PlayState *state, float delta_time) {
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
        Mix_PlayChannel(-1, basket_pong_game->audio->score_sound, 0);
    }

    if (Net_BallInHoop(state->player_two_net, state->ball)) {
        PlayState_OnScore(++state->player_one_score, state->ball, state->player_one_score_label);
        Mix_PlayChannel(-1, basket_pong_game->audio->score_sound, 0);
    }

    if (state->player_one_score == WINNING_SCORE) {
        Game_SwitchState(GAMEOVERSTATE_ID);
        Label_SetText(basket_pong_game->gameover_state->winning_player_label, "Player One Wins!", SDL_TRUE);
    }
    else if (state->player_two_score == WINNING_SCORE) {
        Game_SwitchState(GAMEOVERSTATE_ID);
        Label_SetText(basket_pong_game->gameover_state->winning_player_label, "Player Two Wins!", SDL_TRUE);
    }
}

void PlayState_OnScore(int score, Ball* ball, Label *label) {
    ball->pos_y += BALL_SIZE;
    ball->vel_y /= 2;

    int ndigits = snprintf(NULL, 0, "%d", score);
    char *score_buf = malloc(ndigits + 1);
    sprintf(score_buf, "%d", score);

    SDL_Color color_white = {255, 255, 255, 0};
    Label_SetText(label, score_buf, SDL_FALSE);
    Label_SetColor(label, &color_white);  

    free(score_buf);  
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

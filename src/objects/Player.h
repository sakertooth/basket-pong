#pragma once
#include <SDL2/SDL.h>
#include "Ball.h"

#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 100
#define PLAYER_SPEED 500
#define PLAYER_JUMP_STRENGTH 500
#define PLAYER_GRAVITY 700
#define PLAYER_MAX_SHOOT_POWER 90
#define PLAYER_SHOOT_POWER_RATE 150

struct Player {
    SDL_Scancode move_left_key;
    SDL_Scancode move_right_key;
    SDL_Scancode jump_key;
    SDL_Scancode shoot_ball_key;
    SDL_bool jumping;
    SDL_bool has_ball;
    SDL_bool is_shooting;
    SDL_bool facing_right;
    SDL_Rect shoot_meter_rect;
    float pos_x;
    float pos_y;
    float vel_y;
    float shoot_power;
};


typedef struct Player Player;

Player* Player_Create(int x, int y, SDL_Scancode move_left_key, SDL_Scancode move_right_key, SDL_Scancode jump_key, SDL_Scancode shoot_ball_key);
void Player_Draw(Player* player, SDL_Renderer *renderer);
void Player_Update(Player* player, Ball *ball, float delta_time);

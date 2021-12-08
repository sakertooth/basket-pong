#pragma once
#include <SDL2/SDL.h>

#define BALL_SIZE 10
#define BALL_GRAVITY 150

struct Ball {
    SDL_Rect rect;
    float dribble_speed; 
    float speed;
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    SDL_bool shot;
    SDL_bool taken;
};

typedef struct Ball Ball;

Ball* Ball_Create(int x, int y);
void Ball_Draw(Ball* ball, SDL_Renderer* renderer);
void Ball_Update(Ball *ball, float delta_time);

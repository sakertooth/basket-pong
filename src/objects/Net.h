#pragma once
#include <SDL2/SDL.h>
#include "Ball.h"

#define NET_BOARD_WIDTH 5
#define NET_BOARD_HEIGHT 50

struct Net {
    SDL_Rect backboard_rect;
    SDL_Rect hoop_rect;
    SDL_bool hoop_rotated;
};

typedef struct Net Net;

Net* Net_Create(int x, int y, SDL_bool rotate_hoop);
void Net_Draw(Net* net, SDL_Renderer *renderer);
void Net_Update(Net* net, Ball* ball);
SDL_bool Net_BallInHoop(Net* net, Ball* ball);

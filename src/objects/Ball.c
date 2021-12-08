#include "Ball.h"
#include "Game.h"

Ball* Ball_Create(int x, int y) {
    Ball* ball = malloc(sizeof(Ball));
    if (ball == NULL) {
        return NULL;
    }

    ball->dribble_speed = 200;
    ball->pos_x = (float) x;
    ball->pos_y = (float) y;
    ball->vel_x = 0;
    ball->vel_y = 0;
    ball->shot = SDL_FALSE;
    ball->taken = SDL_FALSE;

    ball->rect.x = x;
    ball->rect.y = y;
    ball->rect.w = BALL_SIZE;
    ball->rect.h = BALL_SIZE;

    return ball;
}

void Ball_Draw(Ball* ball, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    ball->rect.x = (int) ball->pos_x;
    ball->rect.y = (int) ball->pos_y;
    ball->rect.w = BALL_SIZE;
    ball->rect.h = BALL_SIZE;
    SDL_RenderFillRect(renderer, &ball->rect);
}

void Ball_Update(Ball *ball, float delta_time) {
    ball->pos_x += ball->vel_x * delta_time;
    ball->pos_y -= ball->vel_y * delta_time;

    if (ball->shot) {
        if ((int) ball->pos_x <= 0 || (int) ball->pos_x + BALL_SIZE >= SCREEN_WIDTH) {
            float vel_x_normalized = ball->vel_x / SDL_abs(ball->vel_x);
            ball->pos_x -= BALL_SIZE * vel_x_normalized;
            ball->vel_x = -ball->vel_x;
        }

        if (ball->pos_y <= 0) {
            ball->pos_y += BALL_SIZE;
            ball->vel_y = -ball->vel_y;
        }
        
        if ((int) ball->pos_y + BALL_SIZE >= SCREEN_FLOOR) {
            ball->vel_y = 0;
            ball->vel_x = 0;
            ball->shot = SDL_FALSE;
            ball->pos_y = SCREEN_FLOOR - BALL_SIZE;
        }
        else {
            ball->vel_y -= BALL_GRAVITY * delta_time;
        }       
    }
}

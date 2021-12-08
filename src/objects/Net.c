#include "Net.h"

Net* Net_Create(int x, int y, SDL_bool rotate_hoop) {
    Net *net = malloc(sizeof(Net));
    if (net == NULL) {
        return NULL;
    }

    net->hoop_rotated = rotate_hoop;

    net->backboard_rect.x = x;
    net->backboard_rect.y = y;
    net->backboard_rect.w = NET_BOARD_WIDTH;
    net->backboard_rect.h = NET_BOARD_HEIGHT;

    net->hoop_rect.x = rotate_hoop ? x - NET_BOARD_HEIGHT : x + NET_BOARD_WIDTH;
    net->hoop_rect.y = y + NET_BOARD_HEIGHT;
    net->hoop_rect.w = NET_BOARD_HEIGHT;
    net->hoop_rect.h = NET_BOARD_WIDTH;
    return net;
}

void Net_Draw(Net* net, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &net->backboard_rect);
    SDL_RenderFillRect(renderer, &net->hoop_rect);
}

void Net_Update(Net* net, Ball* ball) {
    SDL_Rect backboard_intersection_rect;
    backboard_intersection_rect.x = net->backboard_rect.x + net->backboard_rect.w * (int) !net->hoop_rotated;
    backboard_intersection_rect.y = net->backboard_rect.y;
    backboard_intersection_rect.w = 1;
    backboard_intersection_rect.h = net->backboard_rect.h; 

    SDL_Rect hoop_intersection_rect_bottom;
    hoop_intersection_rect_bottom.x = net->hoop_rect.x;
    hoop_intersection_rect_bottom.y = net->hoop_rect.y + net->hoop_rect.h;
    hoop_intersection_rect_bottom.w = net->hoop_rect.w;
    hoop_intersection_rect_bottom.h = 1;
    
    SDL_Rect hoop_intersection_rect_rim;
    hoop_intersection_rect_rim.x = net->hoop_rect.x + net->hoop_rect.w * (int) !net->hoop_rotated;
    hoop_intersection_rect_rim.y = net->hoop_rect.y;
    hoop_intersection_rect_rim.w = 1;
    hoop_intersection_rect_rim.h = net->hoop_rect.h;


    if (SDL_HasIntersection(&backboard_intersection_rect, &ball->rect) || SDL_HasIntersection(&hoop_intersection_rect_rim, &ball->rect)) {
        ball->vel_x = -ball->vel_x;
        ball->pos_x += BALL_SIZE * (ball->vel_x / SDL_abs(ball->vel_x));
    }

    if (SDL_HasIntersection(&hoop_intersection_rect_bottom, &ball->rect) && ball->vel_y > 0) {
        ball->vel_y = -ball->vel_y;
        ball->pos_y += BALL_SIZE;
    }
}

SDL_bool Net_BallInHoop(Net* net, Ball* ball) {
    SDL_Rect hoop_intersection_rect_top;
    hoop_intersection_rect_top.x = net->hoop_rect.x;
    hoop_intersection_rect_top.y = net->hoop_rect.y;
    hoop_intersection_rect_top.w = net->hoop_rect.w;
    hoop_intersection_rect_top.h = 1;
    return SDL_HasIntersection(&ball->rect, &hoop_intersection_rect_top);
}

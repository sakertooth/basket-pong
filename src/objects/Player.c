#include "Player.h"
#include "Game.h"
#include "../states/PlayState.h"

Player* Player_Create(int x, int y, SDL_Scancode move_left_key, SDL_Scancode move_right_key, SDL_Scancode jump_key, SDL_Scancode shoot_ball_key) {
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        return NULL;
    }

    player->pos_x = x;
    player->pos_y = y;

    player->move_left_key = move_left_key;
    player->move_right_key = move_right_key;
    player->jump_key = jump_key;
    player->shoot_ball_key = shoot_ball_key;

    player->jumping = SDL_FALSE;
    player->has_ball = SDL_FALSE;
    player->is_shooting = SDL_FALSE;

    player->shoot_meter_rect.x = player->pos_x;
    player->shoot_meter_rect.y = player->pos_y - 20;
    player->shoot_meter_rect.w = 0;
    player->shoot_meter_rect.h = 5;
    player->shoot_power = 0;

    return player;
}

void Player_Draw(Player* player, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    
    SDL_Rect player_rect;
    player_rect.x = (int) player->pos_x;
    player_rect.y = (int) player->pos_y;
    player_rect.w = PLAYER_WIDTH;
    player_rect.h = PLAYER_HEIGHT;
    SDL_RenderFillRect(renderer, &player_rect);

    player->shoot_meter_rect.w = (int) player->shoot_power;
    player->shoot_meter_rect.y = (int) player->pos_y - 20;
    player->shoot_meter_rect.x = (int) player->pos_x - (player->shoot_power / 2) + PLAYER_WIDTH / 2;
    SDL_RenderFillRect(renderer, &player->shoot_meter_rect);
}

void Player_Update(Player* player, Ball *ball, float delta_time) {
    const Uint8* pressed_keys = SDL_GetKeyboardState(NULL);

    //Moving
    if (pressed_keys[player->move_left_key] && (player->pos_x > 0 && (!player->has_ball || (player->pos_x > BALL_SIZE + PLAYER_WIDTH && player->has_ball)))) {
        player->pos_x -= PLAYER_SPEED * delta_time;
        player->facing_right = SDL_FALSE;

        if (player->has_ball) {
            ball->pos_x = player->pos_x - PLAYER_WIDTH - 10;
        }

    }
    
    if (pressed_keys[player->move_right_key] && ((!player->has_ball && player->pos_x + PLAYER_WIDTH < SCREEN_WIDTH) || (player->pos_x + 2 * PLAYER_WIDTH + BALL_SIZE < SCREEN_WIDTH && player->has_ball))) {
        player->pos_x += PLAYER_SPEED * delta_time;
        player->facing_right = SDL_TRUE;

        if (player->has_ball) {
            ball->pos_x = player->pos_x + PLAYER_WIDTH + 10;
        }
    }

    //Jumping
    if (pressed_keys[player->jump_key] && !player->jumping) {
        player->jumping = SDL_TRUE;
        player->vel_y = PLAYER_JUMP_STRENGTH;
    }
    
    if (player->jumping) {
        player->pos_y -= player->vel_y * delta_time;
        player->vel_y -= PLAYER_GRAVITY * delta_time;

        if ((int) player->pos_y + PLAYER_HEIGHT >= SCREEN_FLOOR) {
            player->jumping = SDL_FALSE;
            player->vel_y = 0;
            player->pos_y = SCREEN_FLOOR - PLAYER_HEIGHT;
        }

        if (player->has_ball) {
            ball->pos_y = player->pos_y + PLAYER_HEIGHT / 2 - 15;
        }
    }

    //Picking up ball
    int player_center_x = (int) (player->pos_x + PLAYER_WIDTH / 2);
    int player_center_y = (int) (player->pos_y + PLAYER_HEIGHT / 2);
    
    int ball_center_x = (int) (ball->pos_x + BALL_SIZE / 2);
    int ball_center_y = (int) (ball->pos_y + BALL_SIZE / 2);
    
    if (SDL_abs(player_center_x - ball_center_x) <= PLAYER_WIDTH / 2 + BALL_SIZE / 2 &&
        SDL_abs(player_center_y - ball_center_y) <= PLAYER_HEIGHT / 2 + BALL_SIZE / 2 && 
        !player->has_ball && !ball->taken) {
            player->has_ball = SDL_TRUE;
            ball->taken = SDL_TRUE;
            ball->vel_x = 0;
            ball->vel_y = -ball->dribble_speed;
            ball->pos_x = player->pos_x + PLAYER_WIDTH + 10;
            ball->pos_y = player->pos_y + PLAYER_HEIGHT / 2 - 15;
        }

    //Dribbling
    if (player->has_ball && !player->jumping && !player->is_shooting) {
        if ((int) ball->pos_y + BALL_SIZE >= SCREEN_FLOOR) {
            ball->vel_y = ball->dribble_speed;
            ball->pos_y = SCREEN_FLOOR - BALL_SIZE;
        }

        if (ball->pos_y <= (int) player->pos_y + PLAYER_HEIGHT / 2 - 15) {
            ball->vel_y = -ball->dribble_speed;
            ball->pos_y = player->pos_y + PLAYER_HEIGHT / 2 - 15;
        }
    }

    //Shooting
    if (pressed_keys[player->shoot_ball_key] && player->shoot_power < PLAYER_MAX_SHOOT_POWER && player->has_ball) {
        player->shoot_power += PLAYER_SHOOT_POWER_RATE * delta_time;

        if (!player->is_shooting) {
            player->is_shooting = SDL_TRUE;
            ball->shot = SDL_FALSE;
            ball->vel_x = 0;
            ball->vel_y = 0;
            ball->pos_y = player->pos_y + PLAYER_HEIGHT / 2 - 15;
        }
    }
    
    if (player->shoot_power > 0 && !pressed_keys[player->shoot_ball_key]) {
        float shoot_vel_x = player->facing_right ? 5 * player->shoot_power : 5 * -player->shoot_power;
        float shoot_vel_y = 5 * player->shoot_power;

        ball->vel_x = shoot_vel_x;
        ball->vel_y = shoot_vel_y;
        ball->shot = SDL_TRUE;
        ball->taken = SDL_FALSE;
        player->has_ball = SDL_FALSE;
        player->is_shooting = SDL_FALSE;
        player->shoot_power = 0;
    }
}

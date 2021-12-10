#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Label.h"

struct Button {
    SDL_Rect rect;
    Label* label;
    void (*on_click)();
    SDL_Color color;
    SDL_Color hovering_color;
    SDL_bool hovering;
    SDL_bool clicked;
};

typedef struct Button Button;

Button* Button_Create(const char* text, int x, int y, void (*on_click)(void*), TTF_Font* font, SDL_Color* color, SDL_Color* hovering_color, SDL_Renderer* renderer);
void Button_SetPosition(Button* button, int x, int y);
void Button_Draw(Button* button, SDL_Renderer* renderer);
void Button_HandleEvent(Button *button, SDL_Event *event);
void Button_Update(Button* button);
void Button_Free(Button* button);

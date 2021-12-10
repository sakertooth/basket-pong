#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Label {
    SDL_Rect rect;
    SDL_Texture* texture;
    TTF_Font* active_font;
    SDL_Renderer* active_renderer;
    SDL_Color color;
    char *text;
};

typedef struct Label Label;

Label* Label_Create(const char *text, int x, int y, TTF_Font* font, SDL_Color* color, SDL_Renderer* renderer);
void Label_Draw(Label* label, SDL_Renderer* renderer);
void Label_SetColor(Label* label, const SDL_Color* color);
void Label_SetText(Label* label, const char* text, SDL_bool maintain_center);
void Label_Free(Label* label);

#pragma once
#include <SDL2/SDL_ttf.h>

#define FONT_FILE "./assets/font.ttf"

struct Fonts {
    TTF_Font* small_font;    
    TTF_Font* medium_font;    
    TTF_Font* large_font;    
    TTF_Font* title_font;    
};

typedef struct Fonts Fonts;

Fonts* Fonts_Init();
void Fonts_Free(Fonts *fonts);

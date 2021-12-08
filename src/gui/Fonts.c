#include "Fonts.h"

Fonts* Fonts_Init() {
    Fonts *fonts = malloc(sizeof(Fonts));
    if (fonts == NULL) {
        return NULL;
    }

    fonts->small_font = TTF_OpenFont(FONT_FILE, 16);
    if (fonts->small_font == NULL) {
        goto cleanup1;
    }

    fonts->medium_font = TTF_OpenFont(FONT_FILE, 24);
    if (fonts->medium_font == NULL) {
        goto cleanup2;
    }

    fonts->large_font = TTF_OpenFont(FONT_FILE, 36);
    if (fonts->large_font == NULL) {
        goto cleanup3;
    }

    fonts->title_font = TTF_OpenFont(FONT_FILE, 56);
    if (fonts->title_font == NULL) {
        goto cleanup4;
    }

    return fonts;

cleanup4:
    TTF_CloseFont(fonts->large_font);
cleanup3:
    TTF_CloseFont(fonts->medium_font);
cleanup2:
    TTF_CloseFont(fonts->small_font);
cleanup1:
    free(fonts);
    return NULL;
}

void Fonts_Free(Fonts *fonts) {
    if (fonts == NULL) {
        return;
    }

    TTF_CloseFont(fonts->small_font);
    TTF_CloseFont(fonts->medium_font);
    TTF_CloseFont(fonts->large_font);
    TTF_CloseFont(fonts->title_font);
    free(fonts);
}

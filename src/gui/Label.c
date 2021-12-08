#include "Label.h"
#include "Game.h"

Label* Label_Create(const char *text, int x, int y, TTF_Font* const* font, const SDL_Color* color) {
    Label* label = malloc(sizeof(Label));
    if (label == NULL) {
        return NULL;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(*font, text, *color);
    if (surface == NULL) {
        goto cleanup1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (texture == NULL) {
        goto cleanup2;
    }

    char *label_text = SDL_strdup(text);
    if (label_text == NULL) {
        goto cleanup3;
    }

    label->texture = texture;
    label->font = font;
    label->color = *color;
    label->text = label_text;

    label->rect.x = x;
    label->rect.y = y;
    label->rect.w = surface->w;
    label->rect.h = surface->h;

    SDL_FreeSurface(surface);
    return label;

cleanup3:
    SDL_DestroyTexture(texture);
cleanup2:
    SDL_FreeSurface(surface);
cleanup1:
    free(label);
    return NULL;
}

void Label_Draw(Label* label, SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, label->texture, NULL, &label->rect);
}

void Label_Configure(Label* label, const char *text, const SDL_Color* color) {
    SDL_Surface* surface = TTF_RenderText_Solid(*label->font, text, *color);
    if (surface == NULL) {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_FreeSurface(surface);
    label->texture = texture;
}

void Label_SetColor(Label* label, const SDL_Color* color) {
    Label_Configure(label, label->text, color);
    label->color = *color;
}

void Label_SetText(Label* label, const char* text) {
    if (text == NULL || strcmp(label->text, text) == 0) {
        return;
    }

    Label_Configure(label, text, &label->color);
    free(label->text);
    label->text = SDL_strdup(text);
}

void Label_Free(Label* label) {
    if (label == NULL) {
        return;
    }
    
    SDL_DestroyTexture(label->texture);
    free(label->text);
    free(label);
}

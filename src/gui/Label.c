#include "Label.h"
#include "Game.h"

Label* Label_Create(const char *text, int x, int y, TTF_Font* font, SDL_Color* color, SDL_Renderer* renderer) {
    Label* label = malloc(sizeof(Label));
    if (label == NULL) {
        return NULL;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, *color);
    if (surface == NULL) {
        goto cleanup1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        goto cleanup2;
    }

    char *label_text = SDL_strdup(text);
    if (label_text == NULL) {
        goto cleanup3;
    }

    label->texture = texture;
    label->active_font = font;
    label->active_renderer = renderer;
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
    SDL_Surface* surface = TTF_RenderText_Solid(label->active_font, text, *color);
    if (surface == NULL) {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(label->active_renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        return;
    }

    label->rect.w = surface->w;
    label->rect.h = surface->h;

    SDL_FreeSurface(surface);
    label->texture = texture;
}

void Label_SetColor(Label* label, const SDL_Color* color) {
    Label_Configure(label, label->text, color);
    label->color = *color;
}

void Label_SetText(Label* label, const char* text, SDL_bool maintain_center) {
    if (text == NULL || strcmp(label->text, text) == 0) {
        return;
    }

    int x = label->rect.x;
    Label_Configure(label, text, &label->color);

    if (maintain_center) {
        label->rect.x = x - label->rect.w / 2;
    }

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

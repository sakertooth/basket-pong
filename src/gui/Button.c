#include "Button.h"
#include "Game.h"

Button* Button_Create(const char* text, int x, int y, void (*on_click)(void *), void *arg, TTF_Font* font, SDL_Color* color, SDL_Color* hovering_color, SDL_Renderer* renderer) {
    Button* button = malloc(sizeof(Button));
    if (button == NULL) {
        return NULL;
    }

    button->label = Label_Create(text, x, y, font, hovering_color, renderer);
    if (button->label == NULL) {
        free(button);
        return NULL;
    }

    button->on_click = on_click;
    button->on_click_arg = arg;

    button->color = *color;
    button->hovering_color = *hovering_color;
    button->hovering = SDL_FALSE;
    button->clicked = SDL_FALSE;

    button->label->rect.x = button->rect.x + (button->rect.w - button->label->rect.w) / 2;
    button->label->rect.y = button->rect.y + (button->rect.h - button->label->rect.h) / 2;

    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = button->label->rect.w + 50;
    button->rect.h = button->label->rect.h + 15;
    return button;
}

void Button_SetPosition(Button* button, int x, int y) {
    button->rect.x = x;
    button->rect.y = y;
    button->label->rect.x = x + (button->rect.w - button->label->rect.w) / 2;
    button->label->rect.y = y + (button->rect.h - button->label->rect.h) / 2;
}

void Button_Draw(Button* button, SDL_Renderer* renderer) {

    if (button->hovering) {
        SDL_SetRenderDrawColor(renderer, button->hovering_color.r, button->hovering_color.g, button->hovering_color.b, button->hovering_color.a);
        SDL_RenderFillRect(renderer, &button->rect);
        Label_SetColor(button->label, &button->color);
    }
    else {
        SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
        SDL_RenderFillRect(renderer, &button->rect);
        Label_SetColor(button->label, &button->hovering_color);
    }

    Label_Draw(button->label, renderer);
}

void Button_HandleEvent(Button* button, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        button->on_click(button->on_click_arg);
    }
}

void Button_Update(Button* button) {
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (mouse_x > button->rect.x && mouse_x < button->rect.x + button->rect.w &&
        mouse_y > button->rect.y && mouse_y < button->rect.y + button->rect.h) {

        button->hovering = SDL_TRUE;
    }
    else if (button->hovering == SDL_TRUE) {
        button->hovering = SDL_FALSE;
    }
}

void Button_Free(Button* button) {
    if (button == NULL) {
        return;
    }

    Label_Free(button->label);
    free(button);
}

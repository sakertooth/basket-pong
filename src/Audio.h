#pragma once
#include <SDL2/SDL_mixer.h>

struct Audio {
    Mix_Chunk* dribble_sound;
    Mix_Chunk* score_sound;
};

typedef struct Audio Audio;

Audio* Audio_Create();
void Audio_Free(Audio *audio);
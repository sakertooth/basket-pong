#include "Audio.h"

Audio* Audio_Create() {
    Audio *audio = malloc(sizeof(Audio));
    if (audio == NULL) {
        return NULL;
    }
    
    Mix_Chunk *dribble_sound = Mix_LoadWAV("assets/plop.ogg");
    if (dribble_sound == NULL) {
        goto cleanup1;
    }

    Mix_Chunk *score_sound = Mix_LoadWAV("assets/score.wav");
    if (score_sound == NULL) {
        goto cleanup2;
    }

    Mix_VolumeChunk(dribble_sound, 10);
    audio->dribble_sound = dribble_sound;
    audio->score_sound = score_sound;
    return audio;

cleanup2:
    free(dribble_sound);
cleanup1:
    free(audio);
    return NULL;
}

void Audio_Free(Audio *audio) {
    if (audio == NULL) {
        return;
    }

    Mix_FreeChunk(audio->dribble_sound);
    Mix_FreeChunk(audio->score_sound);
    free(audio);
}
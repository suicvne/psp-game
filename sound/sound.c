#include "sound.h"

sound_effect_t** sound_effects;


sound_effect_t* sound_load_sound(const char* path);
void sound_load_sounds();

void sound_initialize()
{
    sound_effects = malloc(sizeof(sound_effect_t) * ((int)SOUND_number_of_sounds));

    sound_load_sounds();
}

sound_effect_t* sound_load_sound(const char* path)
{
    printf("loading effect %s..\n", path);
    sound_effect_t* effect = malloc(sizeof(sound_effect_t));
    #if PSP
    effect->sound = oslLoadSoundFile(path, OSL_FMT_NONE); //NONE means cache in ram. I want this for smaller ones like the textbox
    #elif SDL_VERS
    //TODO:
    #endif
    return effect;
}

void sound_load_sounds()
{
    sound_effects[SOUND_EFFECT_TEXT] = sound_load_sound("res/text.wav");    
}

void sound_play(SOUND_EFFECT_TYPE sound)
{
    #if PSP
    oslStopSound((sound_effects[sound]->sound));
    oslPlaySound((sound_effects[sound]->sound), 1);
    #elif SDL_VERS
    //TODO
    #endif
}

int sound_is_playing(SOUND_EFFECT_TYPE sound)
{
    #if PSP

    #elif SDL_VERS
    #endif
}
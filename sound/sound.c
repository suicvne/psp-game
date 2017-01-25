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
    effect->sound = Mix_LoadWAV(path);
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
    Mix_PlayChannel(-1, sound_effects[sound]->sound, 0); //channel (-1 for first available), effect, number of times to play
    #endif
}

void sound_destroy()
{
  int i;
  for(i = SOUND_EFFECT_TEXT; i < SOUND_number_of_sounds; i++)
  {
    if(sound_effects[i]->sound != NULL)
    {
      #if PSP
      //TODO:
      #elif SDL_VERS
      Mix_FreeChunk(sound_effects[i]->sound);
      #endif
    }
  }
}

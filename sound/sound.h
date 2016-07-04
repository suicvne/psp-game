/**

A common file for playing sound effects during the game.

*/

#ifndef ___SOUND_H____
#define ___SOUND_H____

#if PSP
#include <oslib/oslib.h>
#elif SDL_VERS
//TODO
#endif

typedef enum {

    SOUND_EFFECT_TEXT,



    SOUND_number_of_sounds //keep this last!!!!!!

} SOUND_EFFECT_TYPE;

typedef struct {

#if PSP
OSL_SOUND* sound;
#elif SDL_VERS
//TODO
#endif

} sound_effect_t;

//extern sound_effect_t* sound_effects;
void sound_initialize(); //inits and loads sound effects
void sound_play(SOUND_EFFECT_TYPE sound);

int sound_is_playing(SOUND_EFFECT_TYPE sound);

#endif //___SOUND_H____
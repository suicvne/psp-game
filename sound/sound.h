/**

A common file for playing sound effects during the game.

*/

#ifndef ___SOUND_H____
#define ___SOUND_H____



typedef enum _SOUND_EFFECT_TYPE {

    SOUND_EFFECT_TEXT,



    SOUND_number_of_sounds //keep this last!!!!!!

} SOUND_EFFECT_TYPE;

typedef struct _sound_effect_t {

    #if PSP
    OSL_SOUND* sound;
    #elif SDL_VERS
    Mix_Chunk* sound;
    #endif

} sound_effect_t;

//extern sound_effect_t* sound_effects;
void sound_initialize(); //inits and loads sound effects
void sound_play(SOUND_EFFECT_TYPE sound);

void sound_destroy(); //destroys sounds

#endif //___SOUND_H____

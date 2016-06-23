/*

  Defines common callback functions for interfacing with the PSP's thread system.

*/

#ifndef ___COMMON_H_
#define ___COMMON_H_

#ifdef PSP
#include <pspctrl.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#endif

#ifndef GU_PI
#define GU_PI 3.141593f
#endif

static inline float angleToRad(int angle)
{
  return ((angle * GU_PI) / 180.0f);
}

static inline float radToDegree(float rad)
{
  return (int)((rad * 180) / GU_PI);
}

static inline void reportFatalError(const char* text)
{
  #ifdef PSP
  oslFatalError(text);
  #elif SDL_VERS
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", text, NULL);
  #endif
}
#endif //___COMMON_H_

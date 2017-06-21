#include "system_info.h"

#ifdef SDL_VERS
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#endif

#include "../globals.h"

system_info_t get_system_info()
{
	system_info_t info;

	#ifdef PSP
	//TODO: differentiate between PSP versions
	info.ram = 32; //TODO: mb or kb??
	info.cpu_core_count = 1;
	info.platform = "PSP";
	info.subsystem = "oslibmod";
	#elif SDL_VERS
	SDL_SysWMinfo sdlinfo;
	SDL_GetWindowWMInfo(kSdlWindow, &sdlinfo);

	info.platform = (char*)SDL_GetPlatform();
	switch(sdlinfo.subsystem)
	{
		case SDL_SYSWM_WINDOWS:
			info.subsystem = "Win32";
			break;
		case SDL_SYSWM_COCOA:
			info.subsystem = "Cocoa";
			break;
		case SDL_SYSWM_WAYLAND:
			info.subsystem = "Wayland";
			break;
		case SDL_SYSWM_MIR:
			info.subsystem = "Mir";
			break;
		default:
			info.subsystem = "Other";
			break;
	}

	info.sdl_version_major = sdlinfo.version.major;
	info.sdl_version_minor = sdlinfo.version.minor;
	info.sdl_version_patch = sdlinfo.version.patch;
	//info.ram = SDL_GetSystemRam(); //TODO: fix this on OS X?
	info.cpu_core_count = SDL_GetCPUCount();

	#endif

	return info;
}

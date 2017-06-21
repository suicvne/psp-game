/**

Defines convenience functions for retrieving system information.

*/
#ifndef SYSTEM_INFO_H_
#define SYSTEM_INFO_H_

typedef struct system_info_t {

	int ram;

	#ifdef SDL_VERS
	int sdl_version_major;
	int sdl_version_minor;
	int sdl_version_patch;
	#endif

	char* subsystem; //window manager
	char* platform;
	int cpu_core_count; //logical cores

} system_info_t;

system_info_t get_system_info();

#endif //SYSTEM_INFO_H_

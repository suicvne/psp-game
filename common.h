/*

  Defines common callback functions for interfacing with the PSP's thread system.

*/

#ifndef ___COMMON_H_
#define ___COMMON_H_

#include <pspctrl.h>
#include <pspdebug.h>

static inline float angleToRad(int angle)
{
  return ((angle * GU_PI) / 180.0f);
}

static inline float radToDegree(float rad)
{
  return (int)((rad * 180) / GU_PI);
}

/* Exit callback */
int exit_callback(void)
{
	sceKernelExitGame();

	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", (void *) exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

int HandleError(const char* errorString)
{
	pspDebugScreenInit();
	SetupCallbacks();
	SceCtrlData buttonData;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	int running = 1;
	while(running)
	{
		pspDebugScreenSetXY(2, 2);
		printf("%s\n\nPress <X> to exit.", errorString);

		sceCtrlPeekBufferPositive(&buttonData, 1);
		if(buttonData.Buttons != 0 && (buttonData.Buttons & PSP_CTRL_CROSS))
			running = 0;
		sceDisplayWaitVblankStart();
	}
	sceKernelExitGame();
	return 0;
}

#endif //___COMMON_H_

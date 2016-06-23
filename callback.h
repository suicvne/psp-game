#ifndef ___CALLBACK_H___
#define ___CALLBACK_H___

#ifdef PSP
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

#endif

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
  #ifdef PSP
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
  #endif
	return 0;
}

#endif //___CALLBACK_H___

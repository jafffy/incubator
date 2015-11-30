#include <pthread.h>

#include "libcommon.h"
#include "libdipsw.h"

struct eventmanager_context context;

static void* thread_arduino(void* args)
{
}

static void* thread_forlowpriority(void* args)
{
	while (TRUE) {
		// dipsw check
		if (dipsw_test()) {
			context.ops.OnDipSwChanged();
		}

		usleep(1);
	}

	pthread_exit(0);
}

BOOL eventmanager_init()
{
	if (dipsw_init()) {
		return FALSE;
	}

	return TRUE;
}

void eventmanager_destroy() 
{
	dipsw_destroy();
}

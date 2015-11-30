#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#include "libcommon.h"
#include "libwindowmanager.h"

static struct windowmanager_context* context;
static volatile BOOL 

bool windowmanager_init(struct windowmanager_operation* ops)
{
	context->ops = *ops;
}

void windowmanager_destroy()
{
	unsigned int i;

	for (i = 0; i < context->n_windows; ++i) {
		struct window_context* w = context->windows[i];

		if (w) {
			window_destroy(w);
			free(w);
			w = context->windows[i] = NULL;
		}
	}

	context->ops.OnDestroy(context);
}

void windowmanager_run()
{
	context->is_running = TRUE;

	context->ops.OnCreate(context);

	while (context->is_running) {
		struct window_context* w = context->current_window;

		w->ops.OnUpdate(w, 0.0);
		w->ops.OnDraw(w);
	}
}

void windowmanager_changewindow(unsigned int index)
{
	if (context->current_window) {
		context->current_window->ops.OnExit(context->current_window);
	}

	context->current_window = context->windows[index];
	context->current_window->ops.OnEnter(context->current_window);
}

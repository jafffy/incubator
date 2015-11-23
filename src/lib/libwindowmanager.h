#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include "libwindow.h"

#define MAX_NUM_WINDOW 2

struct windowmanager_context;

struct windowmanager_operation
{
	// You should add window_contexts on this function.
	BOOL (*OnCreate)(struct windowmanager_context*);
	void (*OnDestroy)(struct windowmanager_context*);
};

struct windowmanager_context
{
	struct window_context* windows[MAX_NUM_WINDOW];
	int n_windows;
	struct window_context* current_window;
	struct windowmanager_operation ops;
	BOOL is_running;
};

bool windowmanager_init(struct windowmanager_operation* ops);
void windowmanager_destroy();

void windowmanager_run();
void windowmanager_changewindow(unsigned int index);

#endif // WINDOWMANAGER_H_

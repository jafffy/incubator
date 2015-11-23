#ifndef LIBWINDOW_H_
#define LIBWINDOW_H_

#define MAX_NUM_WINDOW_OBJECT 20

struct window_context;

enum E_WINDOW_OBJECT {
	EWO_BUTTON,
	EWO_IMAGE,
	EWO_LABEL,
	EWO_COUNT
};

struct window_object
{
	E_WINDOW_OBJECT type;
	void* object;
};

struct window_operation
{
	BOOL (*OnEnter)(struct window_context*);
	void (*OnExit)(struct window_context*);
	void (*OnUpdate)(struct window_context*, double);
	void (*OnDraw)(struct window_context*);
};

struct window_context
{
	struct window_operation ops;
	struct window_object* objects[MAX_NUM_WINDOW_OBJECT];
};

BOOL window_init(struct window_context* context, struct window_operation* ops);
void window_destroy(struct window_context* context);

#endif // LIBWINDOW_H_

#include "libcommon.h"
#include "libwindow.h"

BOOL window_init(struct window_context* context, struct window_operation* ops)
{
	context->ops = *ops;
}

void window_destroy(struct window_context* context)
{
	context->ops.OnExit(context);
}

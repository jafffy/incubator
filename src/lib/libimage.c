#include <errno.h>
#include <stdlib.h>

#include "libcommon.h"
#include "libimage.h"
#include "libscreen.h"

struct image_node* image_create(const char* pathname)
{
	struct image_node* e = (struct image_node*)malloc(sizeof(struct image_node));

	read_bmp(pathname, &e->metadata, &e->data, &e->width, &e->height);

	return e;
}

void image_destroy(struct image_node* e)
{
	if (e == NULL) {
		return;
	}

	free(e);
}

void image_put(struct image_node* e, int x, int y)
{
	drawImage(x, y, x + e->width, y + e->height, e->data);
}



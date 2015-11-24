#ifndef LIBIMAGE_H_
#define LIBIMAGE_H_

struct image_node
{
	int x, y;
	int width, height;
	unsigned char* data;
	unsigned char* metadata;
};

struct image_node* image_create(const char* pathname);
void image_destroy(struct image_node* e);

void image_put(struct image_node* e, int x, int y);

#endif // LIBIMAGE_H_

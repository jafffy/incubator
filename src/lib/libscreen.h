#ifndef LIBSCREEN_H_
#define LIBSCREEN_H_

struct screen_context
{
	int width, height;
	int bits_per_pixel;
	int line_length;

	int fb_fd;
	unsigned char* fb_mem;
};

BOOL initScreen();

void destroyScreen();

void clearScreen(unsigned char *fb_mem);

void read_bmp(const char *filename, char **pDib, char **data, int *cols, int *rows)

void drawRect(unsigned char *fb_mem, int sx, int sy, int ex, int ey, unsigned long color);

void drawImage(unsigned char* fb_mem, int sx, int sy, int ex, int ey, unsigned char* image);

#endif // LIBSCREEN_H_

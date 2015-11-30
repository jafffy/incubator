#ifndef LIBSCREEN_H_
#define LIBSCREEN_H_

BOOL initScreen();

void destroyScreen();

void clearScreen(unsigned char *fb_mem);

void read_bmp(const char *filename, char **pDib, char **data, int *cols, int *rows)

void drawRect(unsigned char *fb_mem, int sx, int sy, int ex, int ey, unsigned long color);

void drawImage(unsigned char* fb_mem, int sx, int sy, int ex, int ey, unsigned char* image);

BOOL isTouched(int x, int y);

#endif // LIBSCREEN_H_

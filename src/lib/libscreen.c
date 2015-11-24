#include "libcommon.h"
#include "libscreen.h"

#define FBDEV_FILE "/dev/fb0"
#define INPUT_DEVICE_LIST "/proc/bus/input/devices"

#define EVENT_STR "/dev/input/event"

#define MAX_TOUCH_X 0x740
#define MAX_TOUCH_Y 0x540

static struct screen_context context;

BOOL initScreen()
{
}

void destroyScreen()
{
}


void clearScreen(unsigned char *fb_mem)
{
	int		coor_y;
	int		coor_x;
	unsigned long *ptr = NULL;

	for (coor_y = 0; coor_y < context.height; coor_y++) {
		ptr = (unsigned long*)fb_mem + context.width * coor_y;

		for (coor_x = 0; coor_x < context.width; coor_x++) {
			*ptr++ = 0x000000;
		}
	}
}

void drawRect(unsigned char *fb_mem, int sx, int sy, int ex, int ey, unsigned long color)
{
	int x,y;
	unsigned long *ptr;

	for (y = sy; y < ey; ++y) {
		ptr = (unsigned long*)fb_mem + context.width * y;

		for (x = sx; x < ex; ++x) {
			*(ptr + x) = color;
		}
	}
}

void drawImage(unsigned char* fb_mem, int sx, int sy, int ex, int ey, unsigned char* image)
{
	int x,y;
	unsigned long *ptr;
	unsigned long *img_ptr;

	for (y = sy; y < ey; ++y) {
		ptr = (unsigned long*)fb_mem + context.width * y;
		img_ptr = (unsigned long*)image + context.width * y;

		for (x = sx; x < ex; ++x) {
			*(ptr + x) = *(img_ptr + x);
		}
	}
}

void read_bmp(char *filename, char **pDib, char **data, int *cols, int *rows)
{
    BITMAPFILEHEADER    bmpHeader;
    BITMAPINFOHEADER    *bmpInfoHeader;
    unsigned int    size;
    unsigned char   magicNum[2];
    int     nread;
    FILE    *fp;

    fp  =  fopen(filename, "rb");
    if(fp == NULL) {
        printf("ERROR\n");
        return;
    }

    // identify bmp file
    magicNum[0]   =   fgetc(fp);
    magicNum[1]   =   fgetc(fp);
    printf("magicNum : %c%c\n", magicNum[0], magicNum[1]);

    if(magicNum[0] != 'B' && magicNum[1] != 'M') {
        printf("It's not a bmp file!\n");
        fclose(fp);
        return;
    }

    nread   =   fread(&bmpHeader.bfSize, 1, sizeof(BITMAPFILEHEADER), fp);
    size    =   bmpHeader.bfSize - sizeof(BITMAPFILEHEADER);
    *pDib   =   (unsigned char *)malloc(size);      // DIB Header(Image Header)
    fread(*pDib, 1, size, fp);
    bmpInfoHeader   =   (BITMAPINFOHEADER *)*pDib;

    printf("nread : %d\n", nread);
    printf("size : %d\n", size);

    // check 24bit
    if(BIT_VALUE_24BIT != (bmpInfoHeader->biBitCount))     // bit value
    {
        printf("It supports only 24bit bmp!\n");
        fclose(fp);
        return;
    }

    *cols   =   bmpInfoHeader->biWidth;
    *rows   =   bmpInfoHeader->biHeight;
    *data   =   (char *)(*pDib + bmpHeader.bfOffBits - sizeof(bmpHeader) - 2);
    fclose(fp);
}



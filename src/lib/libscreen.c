#include "libcommon.h"
#include "libscreen.h"

#define FBDEV_FILE "/dev/fb0"
#define INPUT_DEVICE_LIST "/proc/bus/input/devices"

#define EVENT_STR "/dev/input/event"

#define MAX_TOUCH_X 0x740
#define MAX_TOUCH_Y 0x540

static struct {
	int width, height;
	int bits_per_pixel;
	int line_length;

	int fb_fd;
	int touch_fd;
	int mem_size;
	unsigned char* fb_mem;
} context;


BOOL initScreen()
{
	char buf[BUFSIZ];

	sprintf(buf, "%s%d", EVENT_STR, 2);
	context.touch_fd = open(buf, O_RDONLY);

	if (-1 == context.touch_fd) {
		destroyScreen();
	}

	if (access(FBDEV_FILE, F_OK)) {
		printf("%s: access error\n", FBDEV_FILE);
		destroyScreen();

		return FALSE;
	}

	if ((fb_fd = open(FBDEV_FILE, O_RDWR)) < 0) {
		printf("%s: open error\n", FBDEV_FILE);

		destroyScreen();

		return FALSE;
	}

	struct fb_var_screeninfo fbvar;
	struct fb_fix_screeninfo fbfix;

	if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar)) {
		printf("%s: ioctl error - FBIOGET_VSCREENINFO\n", FBDEV_FILE);
		return FALSE;
	}

	if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &fbfix)) {
		printf("%s: ioctl error - FBIOGET_FSCREENINFO\n", FBDEV_FILE);
		return FALSE;
	}

	context.width  			= fbvar.xres;
	context.height 			= fbvar.yres;
	context.bits_per_pixel 	= fbvar.bits_per_pixel;
	context.line_length 	= fbfix.line_length;
	context.fb_fd 			= fb_fd;
	int mem_size = context.width * context.height * 4;
	context.fb_mem			= (unsigned char*)mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, context.fb_fd, 0);

	return TRUE;
}

void destroyScreen()
{
	close(fb_fd);
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



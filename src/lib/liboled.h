#ifndef LIBOLED_H_
#define LIBOLED_H_

typedef struct {
	int  fd ; //디바이스 오픈시 파일넘버 저장
} oledContext; 

static int writeCmd(int size, unsigned short* cmdArr);
static int writeData(int size, unsigned char* dataArr);
static int setCmdlock(int bLock);

void oled_open(); //드라이버 오픈
void oled_destroy(); //드라이버 클로즈
void reset();
void setting_init_oled();
void oled_imageLoading(const char* fileName);

#endif // LIBOLED_H_


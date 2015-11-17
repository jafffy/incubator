#ifndef LIBOLED_H_
#define LIBOLED_H_

typedef struct {
	int  fd ; //디바이스 오픈시 파일넘버 저장
} oledContext; 

void oled_open(); //드라이버 오픈
void oled_destroy(); //드라이버 클로즈

int oled_imageLoading(const char* fileName);

#endif // LIBOLED_H_


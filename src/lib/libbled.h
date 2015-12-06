#ifndef LIBBUSLED_H_
#define LIBBUSLED_H_

struct busledContext{
	int fd;
}busledContext;

void busled_init();
void busled_destroy();
void severity_classified(unsigned char severity);//4입력하면 왼쪽부터 4개 불 들어옴


#endif

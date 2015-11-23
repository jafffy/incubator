#ifndef LIBBUSLED_H_
#define LIBBUSLED_H_

typedef struct{
	int fd;
}busledContext;

void busled_init();
void busled_destroy();
void severity_display(unsigned short severity);


#endif

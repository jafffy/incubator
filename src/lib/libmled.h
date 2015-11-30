#ifndef LIBMLED_H_
#define LIBMLED_H_

typedef struct{
	int fd;
}mledtext;

void mled_init();
void mled_destory();
int kbhit(void);
void num_of_mled(meldtext* context ,int num);

#endif

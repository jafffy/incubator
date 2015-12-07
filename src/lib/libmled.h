#ifndef LIBMLED_H_
#define LIBMLED_H_

struct mledtext{
	int fd;
};

void mled_init();
void mled_destory();
void num_of_display(unsigned int num);

#endif

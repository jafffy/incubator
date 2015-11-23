#ifndef LIBDIPSW_H_
#define LIBDIPSW_H_

typedef struct {
	int fd;
} dipswContext;

void dipsw_init();
void dipsw_destroy();
int num_of_dipsw();

#endif 

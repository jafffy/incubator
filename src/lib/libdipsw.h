#ifndef LIBDIPSW_H_
#define LIBDIPSW_H_

#ifndef BOOL
#define BOOL int
#endif // BOOL

#ifndef TRUE
#define TRUE 1
#endif // TRUe

#ifndef FALSE
#define FALSE 0
#endif // FALSE


typedef struct {
	int fd;
	int curstate;
} dipswContext;

BOOL dipsw_test();
void dipsw_init();
void dipsw_destroy();
int dipsw_curstate();// if all of dipsws are up except 14,15,16, then it returns  0x1FFF. 
#endif 

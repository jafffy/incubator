#ifndef LIBFND_H_
#define LIBFND_H_

struct fndtext{
        int fd;
};

void fnd_init();
void fnd_destory();
void changemode(int dir);
int changeNum(int num);
void fndDisp(int num);

#endif


#ifndef LIBCLED_H_
#define LIBCLED_H_

struct cledContext {
	int fd;
};

void cled_init();//open the cled device driver
void cled_destroy();//close the cled device driver
void cled_write(unsigned short colorArray[4]);//write color to cled 

#endif // LIBCLED_H_

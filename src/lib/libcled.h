#ifndef LIBCLED_H_
#define LIBCLED_H_

typedef struct {
	int fd;
}cledContext;

void cled_open();//open the cled device driver
void cled_destory();//close the cled device driver
void cled_write(unsigned short* colorArray);//write color to cled 

#endif // LIBCLED_H_

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

struct arduino_context
{
        int fd;
        int arduino_check;
        char buf[256];
        float temp;
        float humid;
        int photo;
        int CDS;
        int BPM;
};

void init_arduino();
void destroy_arduino();
void *arduino_loop();

float read_temp();
float read_humid();
int read_photo();
int read_cds();
int read_bpm();
int is_sound();

#endif
~        

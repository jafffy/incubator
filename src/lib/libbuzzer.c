#include "libcommon.h"
#include "libbuzzer.h"

#ifndef DRIVER_NAME
#define DRIVER_NAME			"/dev/cnbuzzer"
#endif // DRIVER_NAME

#ifndef MAX_BUZZER_NUMBER
#define MAX_BUZZER_NUMBER 	36
#endif // DRIVER_NAME

static struct buzzer_context context;

BOOL buzzer_init(int buzzerNo)
{
}

void buzzer_destroy()
{
}

void buzzer_buzz(int buzzerNo, int power)
{
}

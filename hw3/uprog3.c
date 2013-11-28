#include <stdio.h>
#include "tunistd.h"
#include "tty_public.h"

int main3(void);
#define MILLION 1000000
#define DELAY (10 * MILLION)

int main3()
{
    int i;

    for (i=0;i<DELAY;i++)	/* enough time to drain output q */
	;
    write(TTY1,"cccccccccc",10);
    return 6;
}

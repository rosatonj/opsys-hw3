#include <stdio.h>
#include "tunistd.h"
#include "tty_public.h"

int main3(void);
#define MILLION 1000000
#define DELAY (10 * MILLION)

int main3()
{
    sleep(200); /* enough time to drain output q */
    write(TTY1,"cccccccccc",10);
    return 6;
}

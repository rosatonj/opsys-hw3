#include "tunistd.h"
#include "tty_public.h"

#define MILLION 1000000
#define DELAY (10 * MILLION)
int main1(void);

int main1()
{
    char buf[20];

    write(TTY1,"aaaaaaaaaa",10);
    write(TTY1, "zzz", 3);
    read(TTY1, buf, 10);	/* test read for blocking */
    write(TTY1, buf, 10);	
    return 2;
}

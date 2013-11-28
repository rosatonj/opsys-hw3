#include <stdio.h>
#include "tunistd.h"
#include "tty_public.h"

int main2(void);

int main2()
{
  write(TTY1,"bbbbbbbbbb",10);
  return 4;
}

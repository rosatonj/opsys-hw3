/*********************************************************************
*
*       file:           tty.h
*       author:         betty o'neil
*                       Ray Zhang   for CS440 HW2
*       date:           02/19/97
*
*       private header file for tty drivers
*
*/

#ifndef TTY_H
#define TTY_H

#include "queue/queue.h"

struct tty {
  int echoflag;			/* echo chars in read */
  Queue tbuf;                   /* Q for Tx buffer */
  Queue ebuf;                   /* Q for echo buffer */
  Queue rbuf;                   /* Q for Rx buffer */
};

extern struct tty ttytab[];

void ttyinit(int dev);
int ttyread(int dev, char *buf, int nchar);
int ttywrite(int dev, char *buf, int nchar);
int ttycontrol(int dev, int fncode, int val);

#endif

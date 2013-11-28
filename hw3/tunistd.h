/*********************************************************************
*
*       file:           tunistd.h
*       author:         betty o'neil
*       date:           Feb, '97, F02
*
*       API for Tiny UNIX system calls, like unistd.h
*
*/

#ifndef TUNISTD_H
#define TUNISTD_H

#include "tty_public.h"

/* exit from program */
int exit(int exitcode);
/* read nchar bytes into buf from dev */
int read(int dev, char *buf, int nchar);
/* write nchar bytes from buf to dev */
int write(int dev, char *buf, int nchar);
/* misc. device functions (not implemented) */
int control(int dev, int fncode, int val);

#endif

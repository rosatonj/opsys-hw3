/*********************************************************************
*
*       file:           conf.h
*       author:         betty o'neil
*
*       Device table declarations
*
*/

#ifndef IOCONF_H
#define IOCONF_H

struct  device  {               /* device table entry */
    int	dvnum;			/* dev no. */
    void (*dvinit)(int dev);	/* init routine for dev */
    int	(*dvread)(int dev, char *buf, int n);   /* read fn for dev */
    int	(*dvwrite)(int dev, char *buf, int n);  /* write fn for dev */
    int (*dvcontrol)(int dev, int code, int x); /* control fn for dev */
    int	dvbaseport;		/* base port for hardware regs */
    int dvdata;			/* addr of software data */
};

extern	struct	device devtab[]; /* one struct per device */

/* Note this needs to agree with # devs in ioconf.c */
#define NDEVS 2
#endif

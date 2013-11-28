/*********************************************************************
*
*       file:           io.c
*       author:         betty o'neil
*
*       device-independent i/o package for SAPC
*
*/

#include "ioconf.h"
#include "tsystm.h"
/*====================================================================
*
* i/o initialization loop for SAPC devices
*
*/

void ioinit()
{
  int i;

  for (i = 0; i < NDEVS; i++)
    devtab[i].dvinit(i);	/* call device-specific init routine */
}

/*====================================================================
*
*       read function calling routine for SAPC devices
*
*/

int sysread(int dev, char *buf, int nchar)
{
  if (dev < 0 || dev >= NDEVS) return -1;      /* fail */
  return devtab[dev].dvread(dev, buf, nchar); /* call dev-specific routine */
}

/*====================================================================
*
*       write function calling routine for SAPC devices
*
*/
int syswrite(int dev, char *buf, int nchar)
{
  if (dev < 0 || dev >= NDEVS) return -1;       /* fail */
  return devtab[dev].dvwrite(dev, buf, nchar); /* call dev-specific routine */
}

/*====================================================================
*
*       control function calling routine for SAPC devices
*       NYI (not yet implemented)
*/
int syscontrol(int dev, int fncode, int val)  /* note: val could be pointer */
{
  if (dev < 0 || dev >= NDEVS) return -1;          /* fail */
  return devtab[dev].dvcontrol(dev, fncode, val); /* dev-specific routine */
}

/*********************************************************************
*
*       file:           ioconf.c
*       author:         betty o'neil
*
*       device independent I/O switch
*
*/

#include "ioconf.h"
#include "tty.h"

struct	device	devtab[] = {
    {0,ttyinit, ttyread, ttywrite, ttycontrol, 0x3f8,(int)&ttytab[0]}, /* TTY0 */
    {1,ttyinit, ttyread, ttywrite, ttycontrol, 0x2f8,(int)&ttytab[1]},/* TTY1*/
};

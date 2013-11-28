/*********************************************************************
*
*       file:           tty.c
*       author:         betty o'neil
*                       Ray Zhang
*
*       tty driver--device-specific routines for ttys 
*       using traditional transmitter ack method (shut down ints) 
*
*/
#include <stdio.h>  /* for kprintf prototype */
#include <serial.h>
#include <cpu.h>
#include <pic.h>
#include "ioconf.h"
#include "tty_public.h"
#include "tty.h"
#include "debuglog.h"
#include "proc.h"
#include "sched.h"

/* define maximum size of queue */
#define QMAX 6
/* for debug string buffer */
#define BUFLEN 20

/* tell C about the assembler shell routines */
extern void irq3inthand(void), irq4inthand(void);

/* C part of interrupt handlers--specific names called by the assembler code */
extern void irq3inthandc(void), irq4inthandc(void); 

/* the common code for the two interrupt handlers */ 
void irqinthandc(int dev); 

struct tty ttytab[NTTYS];  /* software params/data for each SLU dev */

/*====================================================================
*
*       tty specific initialization routine for COM devices
*
*/

void ttyinit(int dev)
{
    int baseport;
    struct tty *tty;		/* ptr to tty software params/data block */

    baseport = devtab[dev].dvbaseport; /* pick up hardware addr */
    tty = (struct tty *)devtab[dev].dvdata; /* and software params struct */

    if (baseport == COM1_BASE) {
	/* arm interrupts by installing int vec */
	set_intr_gate(COM1_IRQ+IRQ_TO_INT_N_SHIFT, &irq4inthand);
	/* commanding PIC to interrupt CPU for irq 4 (COM1_IRQ) */
	pic_enable_irq(COM1_IRQ);

    } else if (baseport == COM2_BASE) {
	/* arm interrupts by installing int vec */
	set_intr_gate(COM2_IRQ+IRQ_TO_INT_N_SHIFT, &irq3inthand);
	/* commanding PIC to interrupt CPU for irq 3 (COM2_IRQ) */  
	pic_enable_irq(COM2_IRQ);

    } else {
	kprintf("ttyinit: Bad TTY device table entry, dev %d\n", dev);
	return;			/* give up */
    }
    tty->echoflag = 1;		/* default to echoing */

    init_queue( &(tty->tbuf), QMAX ); /* init tbuf Q */
    init_queue( &(tty->ebuf), QMAX ); /* init ebuf Q */
    init_queue( &(tty->rbuf), QMAX ); /* init rbuf Q */

    /* enable interrupts on receiver now and leave them on */
    /* traditional method--turn on TX ints later when data shows up */
    outpt(baseport+UART_IER, UART_IER_RDI); /* RDI, receiver int, only */
}


/*====================================================================
*
*       tty-specific read routine for TTY devices
*
*/

int ttyread(int dev, char *buf, int nchar)
{
    int baseport;
    struct tty *tty;
    int i = 0;
    int saved_eflags;        /* old cpu control/status reg, so can restore it */

    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */

    while (i < nchar) { /* loop until we get user-specified # of chars */
	saved_eflags = get_eflags();
	cli();			/* disable ints in CPU */

	if (queuecount( &(tty->rbuf) )) /* if there is something in rbuf */
	    buf[i++] = dequeue(&(tty->rbuf));    /* copy from ibuf to user buf */

	set_eflags(saved_eflags);     /* back to previous CPU int. status */
    }
    return nchar; 
}


/*====================================================================
*
*       tty-specific write routine for SAPC devices
*       
*/

int ttywrite(int dev, char *buf, int nchar)
{
    int baseport;
    struct tty *tty;
    int i = 0;
    int saved_eflags;
    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */

    saved_eflags = get_eflags();
    cli();			/* disable ints in CPU */
    /* load tx queue some to get started: this doesn't spin */
    while ((i < nchar) && (enqueue( &(tty->tbuf), buf[i])!=FULLQUE)) 
	i++;

    /* now tell transmitter to interrupt (or restart output) */
    outpt( baseport+UART_IER, UART_IER_RDI | UART_IER_THRI); /* enable both */
    /* read and write int's */
    set_eflags(saved_eflags);
    /* START OF HW3 CHANGES: block instead of spin
       keep int's off across enqueue-sleep to avoid lost wakeup problem.
       Other processes can run during this process's sleep */
    cli();
    while ( i < nchar ) {
	while (enqueue( &(tty->tbuf), buf[i])==FULLQUE) {
	    /* HW3: block process for a while */;
	    /* once blocking, don't need the following in this loop: */
	    set_eflags(saved_eflags);  /* allow ints for a moment */
	    cli();
	}
	/* here having enqueued a char successfully */
	/* make sure transmit ints are on now */
	outpt( baseport+UART_IER, UART_IER_RDI | UART_IER_THRI); 
	i++;			/* success, advance one spot */
    }
    set_eflags(saved_eflags);  /* restore CPU flags */
    return nchar;
}


/*====================================================================
*
*       tty-specific control routine for TTY devices
*
*/

int ttycontrol(int dev, int fncode, int val)
{
    struct tty *this_tty = (struct tty *)(devtab[dev].dvdata);

    if (fncode == ECHOCONTROL)
	this_tty->echoflag = val;
    else return -1;
    return 0;
}

void irq4inthandc()
{
    irqinthandc(TTY0);
}                              
  
void irq3inthandc()
{
    irqinthandc(TTY1);
}                              


/*====================================================================
*
*       tty-specific interrupt routine for COM ports
*
*/
  /* Traditional UART treatment: check the devices' ready status
     on int, shut down tx ints if nothing more to write.
     Note: only accesses UART_IIR to satisfy VMWare requirement */

void irqinthandc(int dev)
{
    int ch, lsr;
    char buf[BUFLEN];
    struct tty *tty = (struct tty *)(devtab[dev].dvdata);
    int baseport = devtab[dev].dvbaseport; /* hardware i/o port */

    pic_end_int();                /* notify PIC that its part is done */
    inpt(baseport+UART_IIR); /* VMWare requires this as first action here */
    if ((lsr = inpt(baseport+UART_LSR)) & UART_LSR_DR) { /* if it's read ready */
	ch = inpt(baseport+UART_RX); /* read char, ack the device */
	enqueue( &tty->rbuf, ch );	/* save char in write Q */
	if (tty->echoflag){	/* if echoing wanted */
	    sprintf(buf,"^%c", ch);   /* record input char-- */
	    debuglog(buf);		/* --in debug log in memory */
	    enqueue(&tty->ebuf,ch);	/* echo char */
	    if (queuecount( &tty->ebuf )==1)  /* if first char...*/
		/* enable transmit interrupts also */      
		outpt( baseport+UART_IER, UART_IER_RDI | UART_IER_THRI);
	}
    }
    if (lsr & UART_LSR_THRE) { /* if it's tx ready */
	if (queuecount( &tty->ebuf )) { /* if there is char in echo Q output it*/
	    outpt( baseport+UART_TX, dequeue( &tty->ebuf ) ); /* ack tx dev */
	    debuglog("~e");		/* mark echo with ~e */
	}
	else if (queuecount( &tty->tbuf )) {
	    /* if there is char in tbuf Q output it */
	    ch = dequeue(&tty->tbuf);
	    outpt( baseport+UART_TX, ch ); /* output it, ack tx dev */
	    /* mark ordinary output with ~<char> in debuglog */
	    sprintf(buf,"~%c", ch);
	    debuglog(buf);
	    /* HW3 change: unblock processes waiting for this event */
	} else {			/* all done transmitting */
	    outpt( baseport+UART_IER, UART_IER_RDI); /* shut down tx ints */
	    debuglog("~S");		/* mark shutdown with ~S */
	}
    } 
}



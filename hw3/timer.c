/* 
 * file   : timer.c 
 * purpose: hw5 timer routines 
 * author : Dana C. Chandler III
 */
#include <stdio.h>
#include "timer.h"
#include <cpu.h>
#include <pic.h>
#include <timer.h>
#include "tsystm.h"
#include "proc.h"
#include "sched.h"
#include "debuglog.h"
#include "ttimer.h"

/* procedure prototype */
static void set_timer_count( int );
static void small_delay(void);
void irq0inthandc(void);
void tick_handler(void);

extern IntHandler irq0inthand; /* assembler envelope */

#define TEN_MIL_SEC 0x2e9a

/* init_ticks: initiate the PIT
 * save the interval statically
 * Note: should be called with interrupts off
 */
void init_ticks()
{
    /* set the interrupt gate */
    /* irq 0 maps to slot n = 0x20 in IDT for linux setup */
    set_intr_gate(TIMER0_IRQ+IRQ_TO_INT_N_SHIFT, &irq0inthand);
  
    /* enable the PIC for timer IRQ */	
    pic_enable_irq(TIMER0_IRQ);
	
    /* set up the timer down count from 10 msec*/
    set_timer_count(TEN_MIL_SEC); 
}
/*
 * shutdown_tick: shut off interval timer
 * Note: should be called with interrupts off
 */
void shutdown_ticks()
{
    pic_disable_irq(TIMER0_IRQ);	/* disallow irq 0 ints to get to CPU */
}

/* 
 * Set up timer to count down from given count, then send a tick interrupt, 
 * over and over. A count of 0 sets max count, 65536 = 2**16 
 */

void set_timer_count(int count)
{
    outpt(TIMER_CNTRL_PORT, TIMER0|TIMER_SET_ALL|TIMER_MODE_RATEGEN);
    outpt(TIMER0_COUNT_PORT,count&0xff); /* set LSB here */
    outpt(TIMER0_COUNT_PORT,count>>8);   /* and MSB here */
    small_delay();		       /* give the timer a moment to init. */
}

/* 
 * about 10 us on a SAPC (400Mhz Pentium) 
 */
void small_delay(void)
{
    int i;
  
    for (i=0;i<1000;i++)
	;
}

/* timer interrupt handler 
 */
void irq0inthandc(void)
{
    pic_end_int();		/* notify PIC that its part is done */
    tick_handler();		
}

void tick_handler()
{
    char buf[100];

    sprintf(buf,"*%d", curproc-proctab);
    kprintf("*");
    debuglog(buf);
}


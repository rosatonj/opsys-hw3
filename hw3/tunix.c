/* file: tunix.c core kernel code */

#include <cpu.h>
#include <gates.h>
#include <pic.h>
#include <serial.h>
#include "tsyscall.h"
#include "tsystm.h"
#include "debuglog.h"
#include "proc.h"
#include "sched.h"

extern IntHandler syscall; /* the assembler envelope routine    */
extern void ustart1(void),ustart2(void), ustart3(void);
extern void finale(void);

/* kprintf is proto'd in stdio.h, but we don't need that for anything else */
void kprintf(char *, ...);	

/* functions in this file */
void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug);
void set_trap_gate(int n, IntHandler *inthand_addr);
int sysexit(int);
void k_init(void);
void shutdown(void);
void syscallc( int user_eax, int devcode, char *buff , int bufflen);
void process0(void);
void init_proctab(void);

/* Record debug info in memory between program and stack */
/* 0x300000 = 3M, the start of the last M of user memory on the SAPC */
#define DEBUG_AREA ((char *)0x300000)
#define DEBUG_AREA_LENGTH 0x40000  /* up to 0x340000, 256KB */
#define DELAY_COUNT (40*1000*1000)
/* should be in cpu.h: bit position of IF in EFLAGS */
#define IF_BIT 9

/* for saved esp register--stack bases for user programs */
/* could use enum here */
#define STACKBASE1 0x3f0000
#define STACKBASE2 0x3e0000
#define STACKBASE3 0x3d0000

/* for saved eflags register initialization */
#define EFLAGS_IF (1<<IF_BIT)

/* kernel globals added for scheduler */
PEntry proctab[NPROC],*curproc; 
int number_of_zombie;
int system_time;		/* number of millisecs since startup */

#define MAX_CALL 6

/* syscall dispatch table */
static  struct sysent {
    short   sy_narg;        /* total number of arguments */
    int     (*sy_call)(int, ...);   /* handler */
} sysent[MAX_CALL];

/* end of kernel globals */


/****************************************************************************/
/* k_init: this function for the initialize  of the kernel system*/
/****************************************************************************/

void k_init(){
    init_debuglog(DEBUG_AREA, DEBUG_AREA_LENGTH);	/* specify debug logging area in memory */
    cli();
    init_ticks();
    ioinit();            /* initialize the deivce */ 
    set_trap_gate(0x80, &syscall);   /* SET THE TRAP GATE*/
    system_time = 0;

    /* Note: Could set these with initializers */
    /* Need to cast function pointer type to keep ANSI C happy */
    sysent[TREAD].sy_call = (int (*)(int, ...))sysread;
    sysent[TWRITE].sy_call = (int (*)(int, ...))syswrite;
    sysent[TEXIT].sy_call = (int (*)(int, ...))sysexit;
    sysent[TEXIT].sy_narg = 1;    /* set the arg number of function */
    sysent[TREAD].sy_narg = 3;
    sysent[TIOCTL].sy_narg = 3;
    sysent[TWRITE].sy_narg = 3;

    /*    sysent[TSLEEP].sy_call = (int (*)(int, ...))syssleep; */
    sysent[TSLEEP].sy_narg = 1;    /* set the arg number of function */
    curproc = &proctab[0];
    init_proctab();
    process0();			/* rest of kernel operation (non-init) */
}

/****************************************************************************/
/* process0:  code for process 0: runs when necessary, shuts down            */
/****************************************************************************/
void process0()
{
    int i;
    /* HW3 TODO: fix this up to do normal scheduling */
    for (i=1; i<=3; i++) {	/* do one user process at a time */
	while (proctab[i].p_status != ZOMBIE) {
	    sti();		/* let proc 0 take interrupts (important!) */
	    cli();		/* but just for a moment */
	    schedule_one();
	}
    }
    kprintf("SHUTTING DOWN\n");
    sti();
    for (i=0; i< 1000000; i++)
	;				/* let output finish (kludge) */
  
    for(i=1;i<NPROC;i++)
	kprintf("\nEXIT CODE OF PROCESS %d: %d\n",i,proctab[i].p_exitval);
  
    shutdown();
    /* note that we can return, in process0, to the startup module
       with its int $3.  It's OK to jump to finale, but not necessary */
}
/****************************************************************************/
/* init_proctab: this function for setting init_sp, init_pc              */
/* zeroing out savededp, and set to RUN                                     */
/****************************************************************************/
void init_proctab()
{
    int i;

    proctab[1].p_savedregs[SAVED_PC] = (int)&ustart1;
    proctab[2].p_savedregs[SAVED_PC] = (int)&ustart2;
    proctab[3].p_savedregs[SAVED_PC] = (int)&ustart3;
 
    proctab[1].p_savedregs[SAVED_ESP] = STACKBASE1;
    proctab[2].p_savedregs[SAVED_ESP] = STACKBASE2;
    proctab[3].p_savedregs[SAVED_ESP] = STACKBASE3;

    for(i=0;i<NPROC;i++){
	proctab[i].p_savedregs[SAVED_EBP] = 0;
	proctab[i].p_savedregs[SAVED_EFLAGS] = EFLAGS_IF; /* make IF=1 */
	proctab[i].p_status=RUN;
    }

    curproc=&proctab[0];
 
    number_of_zombie=0;
}

/* shut the system down */
void shutdown()
{
    int i;

    /* let output finish (was missing in supplied files) */
    for (i=0; i< DELAY_COUNT; i++)
	;
    cli();
    shutdown_ticks();		/* disable timer interrupts */
    pic_disable_irq(COM1_IRQ);	/* disable COM interrupts in use */
    pic_disable_irq(COM2_IRQ);
    sti();			/* normal for SAPC: interrupts on */
    kprintf("SHUTTING THE SYSTEM DOWN!\n");
    kprintf("Debug log from run:\n");
    kprintf("Marking kernel events as follows:\n");
    kprintf("  ^a   COM2 input interrupt, a received\n");
    kprintf("  ~a   COM2 output interrupt, a output\n");
    kprintf("  ~e   COM2 output interrupt, echo output\n");
    kprintf("  ~S   COM2 output interrupt, shutdown TX ints\n");
    if (get_debuglog_wrapcount() > 0)
	kprintf("Note: This debuglog output is incomplete due to wrapping in the debuglog area\n"); 
    kprintf("  |(1z-2) process switch from 1, now a zombie, to 2\n");
    kprintf("  |(1b-2) process switch from 1, now blocked, to 2\n");
    kprintf("  |(0-1) process switch from 0 to 1\n");
    kprintf("-----------------DEBUGLOG-------------------------------------\n");
    kprintf("%s", get_debuglog_string());   /* the debug log from memory */
    kprintf("\n--------------------------------------------------------------\n");
    kprintf("\nLEAVE KERNEL!\n\n");
    finale();		/* trap to Tutor */
}

/****************************************************************************/
/* syscallc: this function for the C part of the 0x80 trap handler          */
/* OK to just switch on the system call number here                         */
/* By putting the return value of syswrite, etc. in user_eax, it gets       */
/* popped back in sysentry.s and returned to user in eax                    */
/****************************************************************************/

void syscallc( int user_eax, int devcode, char *buff , int bufflen)
{
    int nargs;
    int syscall_no = user_eax;

    switch(nargs = sysent[syscall_no].sy_narg)
	{
	case 1:         /* 1-argument system call */
	    user_eax = sysent[syscall_no].sy_call(devcode);   /* sysexit */
	    break;
	case 3:         /* 3-arg system call: calls sysread or syswrite */
	    user_eax = sysent[syscall_no].sy_call(devcode,buff,bufflen); 
	    break;
	default: kprintf("bad # syscall args %d, syscall #%d\n",
			 nargs, syscall_no);
	}
} 

/****************************************************************************/
/* sysexit: this function for the exit syscall function */
/****************************************************************************/

int sysexit(int exit_code)
{ 
    cli();
    curproc->p_exitval = exit_code;
    curproc->p_status = ZOMBIE;
    number_of_zombie++; 
    schedule_one(curproc-proctab); /* will switch to process 0 */
    /* never returns */       
    return 0;    /* never happens, but avoids warning about no return value */
}

/****************************************************************************/
/* set_trap_gate: this function for setting the trap gate */
/****************************************************************************/

void set_trap_gate(int n, IntHandler *inthand_addr)
{
    debug_set_trap_gate(n, inthand_addr, 0);
}

/* write the nth idt descriptor as a trap gate to inthand_addr */
void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug)
{
    char *idt_addr;
    Gate_descriptor *idt, *desc;
    unsigned int limit = 0;
    extern void locate_idt(unsigned int *, char **);

    if (debug)
	kprintf("Calling locate_idt to do sidt instruction...\n");
    locate_idt(&limit,&idt_addr);
    /* convert to CS seg offset, i.e., ordinary address, then to typed pointer */
    idt = (Gate_descriptor *)(idt_addr - KERNEL_BASE_LA);
    if (debug)
	kprintf("Found idt at %x, lim %x\n",idt, limit);
    desc = &idt[n];               /* select nth descriptor in idt table */
    /* fill in descriptor */
    if (debug)
	kprintf("Filling in desc at %x with addr %x\n",(unsigned int)desc,
		(unsigned int)inthand_addr);
    desc->selector = KERNEL_CS;   /* CS seg selector for int. handler */
    desc->addr_hi = ((unsigned int)inthand_addr)>>16; /* CS seg offset of inthand */
    desc->addr_lo = ((unsigned int)inthand_addr)&0xffff;
    desc->flags = GATE_P|GATE_DPL_KERNEL|GATE_TRAPGATE; /* valid, trap */
    desc->zero = 0;
}


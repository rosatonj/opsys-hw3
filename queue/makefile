#  build queue package and its testbed

PC_INC = $(SAPC_TOOLS)/include

PC_CC   = $(SAPC_GNUBIN)/i386-gcc
PC_CFLAGS = -g -Wall -Wno-implicit -Wshadow -I$(PC_INC)
PC_LD   = $(SAPC_GNUBIN)/i386-ld


testqueue: queue.h queue.c testqueue.c
	gcc $(PC_CFLAGS) testqueue.c queue.c -o testqueue

queue.opc: queue.h queue.c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o queue.opc queue.c

clean:
	 rm -f *.opc *.o *.lnx syms


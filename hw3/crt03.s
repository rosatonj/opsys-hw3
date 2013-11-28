#crt03.s: startup main3
	
.globl _ustart3
.text
			
_ustart3:
	call _main3                  #call main3 in the uprog3.c
        pushl %eax                   #push the retval=exit_code on stack
        call _exit                   #call sysycall exit
               


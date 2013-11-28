#crt02.s: startup main2

.globl _ustart2
.text
			
_ustart2:
	call _main2                  #call main in the uprog2.c
        pushl %eax                   #push the retval=exit_code on stack
        call _exit                   #call sysycall exit
               


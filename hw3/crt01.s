#crt01.s: startup main1
	
.globl _ustart1
.text
			
_ustart1:	
	call _main1                  #call main1 in the uprog1.c
        pushl %eax                   #push the retval=exit_code on stack
        call _exit                   #call sysycall exit
               


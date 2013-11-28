#file:	 sysentry.s
#name:	 joseph tam, paul hui
#date    3-15-97
# description :	 trap handler			
 
.globl _syscallc, _syscall      

_syscall:       pushl %edx        # push the value of eax to edx to stack
                pushl %ecx
                pushl %ebx
                pushl %eax
                call _syscallc    # call c trap routine in tunix.c
                addl $4, %esp     # use the new value of eax ,so adjust the
                popl %ebx         # stack instead of pop eax and then
                popl %ecx         # pop the value of ebx to edx back to stack
                popl %edx
                iret





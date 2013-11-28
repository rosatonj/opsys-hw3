# file :	ulib.s
# description:	assembler routines for read, write, exit

# following Linux system call linkage
# user stack when execution reaches _write:	
# esp-> return addr
#       first arg  (to be put in ebx for syscall)
#       second arg (to be put in ecx)
#       third arg  (to be put in edx)

# user stack after pushl %ebx, needed to preserve %ebx (not a C scratch reg)
# esp->  saved-ebx
# 4(esp) return addr
# 8(esp) first arg  (to be put in ebx for syscall)
#12(esp) second arg (to be put in ecx)
#16(esp) third arg  (to be put in edx)
			
.globl _write,_read,_ioctl,_exit	
.text
	
_write:	pushl %ebx                    # save the value of ebx
	movl 8(%esp),%ebx              
	movl 12(%esp),%ecx
	movl 16(%esp),%edx
	movl $3,%eax
        int $0x80                     # call trap handler
	popl  %ebx                    # restore the value of ebx
	ret

_read:	pushl %ebx                       # save the value of ebx
	movl 8(%esp),%ebx
	movl 12(%esp),%ecx
	movl 16(%esp),%edx
	movl $2,%eax
        int $0x80                     # call trap handler      
	popl  %ebx                    # restore the value of ebx
	ret
	
_ioctl:	pushl %ebx                     # save the value of ebx
	movl 8(%esp),%ebx
	movl 12(%esp),%ecx
	movl 16(%esp),%edx
	movl $5,%eax
        int $0x80                     # call trap handler
	popl  %ebx                    # restore the value of ebx
	ret
			
_exit:	pushl %ebx                      # save the value of ebx
	movl 8(%esp),%ebx
	movl $1,%eax          
        int $0x80                      # call trap handler
	popl  %ebx                     # restore the value of ebx
	ret	



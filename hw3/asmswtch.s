# asmswtch.s:	 switch CPU context for multitasking
# the job to do here:	
# --save CPU registers in previous-process's PEntry
# --restore CPU registers from next-process's PEntry
# all this while running using the CPU to do it's own reconfiguration

.globl _asmswtch

# asmswtch--process switch
#  (but we don't need to save C scratch reg's, since they are assumed
#  to be smashed by calling this anyway)
#  Call from C:	two arguments, pointers to old and new PEntry's:
#
#	asmswtch(oldpentryp, newpentryp)

#  Stack when reach here:
#  %esp-->  return pc
#  4(%esp)  oldpentryp
#  8(%esp)  newpentryp

#  PEntry:	saved esp
#		saved ebx
#		saved esi
#		saved edi
#		saved ebp
#		saved eflags
#		saved pc -- i.e., saved eip (where this was called from)

_asmswtch:
	movl 4(%esp),%eax     # oldpentryp
	movl %esp, (%eax)     # save %esp at start of PEntry
	movl %ebx, 4(%eax)    # followed by other non-scratch regs
	movl %esi, 8(%eax)
	movl %edi, 12(%eax)
	movl %ebp, 16(%eax)
	movl (%esp), %ecx
	movl %ecx, 24(%eax)   # save ret addr=top of stack
	pushfl                # push eflags on stack
	popl 20(%eax)         # pop into saved eflags
	movl 8(%esp), %eax    # newpentryp
	movl (%eax), %esp     # new stack pointer
	movl 4(%eax), %ebx    # restore from saved regs
	movl 8(%eax), %esi
	movl 12(%eax), %edi
	movl 16(%eax), %ebp
	pushl 20(%eax)        # new eflags--push on stack
	popfl		      # restore eflags
	movl 24(%eax), %ecx
	movl %ecx,(%esp)      # restore top of stack=saved pc
	ret		      # return on newly reinstated stack

	



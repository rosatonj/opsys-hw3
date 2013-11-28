# file:	 startup0.s
# asm startup file
# very first module in load

.globl _finale
.text
	movl $0x3ffff0, %esp  # set initial kernel stack
	call _startupc         # call C startup

_finale: int $3                #  return to Tutor
	



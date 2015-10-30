[BITS 32]
global kswitch_and_execute_kernel

SECTION .text

kswitch_and_execute_kernel:
	mov eax, 0x100000 ; set cr3 to 0x100000(addr)
	mov cr3, eax

	mov eax, cr0
	or  eax, 0x80000000 ; enable PG
	mov cr0, eax

	ret

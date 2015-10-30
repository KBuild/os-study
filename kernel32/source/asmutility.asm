[BITS 32]

SECTION .text

; use in C
global kinport_byte, koutport_byte

; read 1byte from port
kinport_byte:
	push ebp
	mov ebp, esp

	push edx
	mov edx, dword[ebp+8]
	mov eax, 0

	in al, dx

	pop edx
	pop ebp
	ret

;write 1byte to port
koutport_byte:
	push ebp
	mov ebp, esp

	push edx
	push eax

	mov edx, dword[ebp+8]
	mov eax, dword[ebp+12]
	out dx, al

	pop eax
	pop edx
	pop ebp
	ret

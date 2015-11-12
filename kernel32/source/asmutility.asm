[BITS 32]

SECTION .text

; use in C
global kinport_byte, koutport_byte, kload_gdtr, kload_tr, kload_idtr

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

; load GDTR register, set to GDT table
kload_gdtr:
	push ebp
	mov ebp, esp

    push eax

    mov eax, dword[ebp+8]
    lgdt[ eax ]

    pop eax
    pop ebp
    ret

; load TR register, set to TSS segment
kload_tr:
	push ebp
	mov ebp, esp

    push edi

    mov edi, dword[ebp+8]
    ltr di

    pop edi
    pop ebp
    ret

; load IDTR register, set to IDT table
kload_idtr:
	push ebp
	mov ebp, esp

    push eax

    mov eax, dword[ebp+8]
    lidt [ eax ]

    pop eax
    pop ebp
    ret

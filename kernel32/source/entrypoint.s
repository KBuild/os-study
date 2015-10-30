[ORG 0x00]
[BITS 16]

SECTION .text

;;;;;;;;;;
; CODE SECTION
;;;;;;;;;;

START:
	mov ax, 0x1000 ; start address of entry point
	mov ds, ax ; set 0x1000 as Data Segment(DS)
	mov es, ax ; and Data Segment(ES)

;;;;;;;;;;
; A20 Gate enabler
; if fail to switch BIOS, then protect
;;;;;;;;;;
	mov ax, 0x2401
	int 0x15

	jc .A20GATEERROR
	jmp .A20GATESUCCESS

.A20GATEERROR: ; if fail then siwtch to system control port
	in al, 0x92 ; read system control port(0x92)
	or al, 0x92 ; set bit to 1(bit one)
	and al, 0xFE ; set to 0(bit zero)
	out 0x92, al ; set system control port to 1byte value

.A20GATESUCCESS:
	cli ; block interrupt
	lgdt[ GDTR ] ; set GDTR to processor, load GDT table

;;;;;;;;;;
; ENTER TO PROTECTION MODE
; Disable Paging, Disable Cache, Internal FPU, Disable Align Check
;;;;;;;;;;
	mov eax, 0x4000003B ; PG=0 CD=1 NW=0 AM=0 WP=0 NE=1 ET=1 TS=1
	                    ; EM=0 MP=1 PE=1
	mov cr0, eax ; set CR0 flag

	jmp dword 0x08:(PROTECTEDMODE - $$ + 0x10000)

;;;;;;;;;;
; enter as 32bit protected
;;;;;;;;;;
[BITS 32]
PROTECTEDMODE:
	mov ax, 0x10 ; set "kernel data segment discriptor" to AX
	mov ds, ax ; set to DS (segment selector)
	mov es, ax ; set to ES (segment selector)
	mov fs, ax ; set to FS (segment selector)
	mov gs, ax ; set to GS (segment selector)

	;; make 64KB stacck to 0x00000000~0x0000FFFF
	mov ss, ax ; set to stack segment
	mov esp, 0xFFFE ; set ESP register address
	mov ebp, 0xFFFE ; set ESP register address

	;; PRINT MESSAGE FOR ALERT PROTECTED MODE
	push(SWITCHSUCCESSMESSAGE - $$ + 0x10000)
	push 2
	push 0
	call PRINTMESSAGE
	add esp, 12

	jmp dword 0x08:0x10200 ; jump to c lang kernel

;;;;;;;;;;
; FUNCTION CODE SECTION
;;;;;;;;;;

;;print a string(message) (32bit function)
; PARAM : position x, position y, string
PRINTMESSAGE:
	push ebp ; backup ebp
	mov ebp, esp ; set esp to ebp
	push esi
	push edi
	push eax
	push ecx
	push edx ; backup registers

	;; get y position
	mov eax, dword[ebp+12] ; load from parameter
	mov esi, 160
	mul esi ; multiply esi to eax
	mov edi, eax ; set to edi

	;; get x position
	mov eax, dword[ebp+8]
	mov esi, 2
	mul esi ; multiply esi to eax
	add edi, eax ; add to edi

	mov esi, dword[ebp+16]

.MESSAGELOOP:
	mov cl, byte[esi]
	cmp cl, 0

	je .MESSAGEEND

	mov byte[edi+0xB8000], cl ; print to 0xB8000 + EDI

	add esi, 1
	add edi, 2

	jmp .MESSAGELOOP

.MESSAGEEND:
	pop edx
	pop ecx
	pop eax
	pop edi
	pop esi
	pop ebp ; restore register
	ret ; return to code(callee)

;;;;;;;;;;
; DATA SECTION
;;;;;;;;;;
	align 8, db 0 ; data align with 8byte

	dw 0x0000 ; align end of GDTR

	GDTR:
		dw GDTEND - GDT - 1
		dd (GDT - $$ + 0x10000)

	;define GDT table
	GDT:
		NULLDESCRIPTOR:
			dw 0x0000
			dw 0x0000
			db 0x00
			db 0x00
			db 0x00
			db 0x00

		;kernel code segment discriptor for protected mode
		CODEDESCRIPTOR:
			dw 0xFFFF ; Limit[15:0]
			dw 0x0000 ; Base[15:0]
			db 0x00   ; Base[23:16]
			db 0x9A   ; P=1, DPL=0, Code Segment, Execute/Read
			db 0xCF   ; G=1, D=1, L=0, Limit[19:16]
			db 0x00   ; Base[31:24]

		DATADESCRIPTOR:
			dw 0xFFFF ; Limit[15:0]
			dw 0x0000 ; Base[15:0]
			db 0x00   ; Base[23:16]
			db 0x92   ; P=1, DPL=0, Code Segment, Read/Write
			db 0xCF   ; G=1, D=1, L=0, Limit[19:16]
			db 0x00   ; Base[31:24]
	GDTEND:

SWITCHSUCCESSMESSAGE: db 'Switch To Protected Mode Success', 0

times 512 - ($ - $$) db 0x00 ; fill to 512byte with '0'

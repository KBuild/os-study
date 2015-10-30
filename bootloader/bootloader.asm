[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x07C0:START

;;;;;;;;;;;;;;;;;;;;;;
; OS configuration value
;;;;;;;;;;;;;;;;;;;;;;
TOTALSECTORCOUNT: dw 0x02 ; OS image size(without Bootloader)

;;;;;;;;;;;;;;;;;;;;;;
; Code Section
;;;;;;;;;;;;;;;;;;;;;;
START:
	mov ax, 0x07C0 ; set 07C0 Address
	mov ds, ax ; set Data Segment(ES)
	mov ax, 0xB800 ; set B800 Address
	mov es, ax ; set Data Segment(ES)

	;generate stack(0x0000:0000 ~ 0x0000:FFFF)
	mov ax, 0x0000
	mov ss, ax ; set stack segment
	mov sp, 0xFFFE ; set SP register
	mov bp, 0xFFFE ; set BP register

	;;;;;;;;;;
	; clean screen
	;;;;;;;;;;
	mov si, 0

.SCREENCLEARLOOP:
	mov byte[es:si], 0
	mov byte[es:si+1], 0x07
	add si, 2
	cmp si, 80 * 25 * 2 ;screen size
	jl .SCREENCLEARLOOP ;looping

	;;print MESSAGE1
	push MESSAGE1
	push 0
	push 0
	call PRINTMESSAGE
	add  sp, 6

	;;print IMAGELOADINGMESSAGE
	push IMAGELOADINGMESSAGE
	push 1
	push 0
	call PRINTMESSAGE
	add  sp, 6

RESETDISK: ;; start disk reset code
	mov ax, 0
	mov dl, 0
	int 0x13 ; call interrupt 13 ( BIOS DISK I/O SERVICE )
	jc HANDLEDISKERROR ; jump to exception(disk error)
	
	mov si, 0x1000
	mov es, si ; set Data Segment(ES) 0x1000
	mov bx, 0x0000 ; finally address 0x1000:0x0000

	mov di, word[TOTALSECTORCOUNT]

READDATA: ;; start disk readding code
	cmp di, 0
	je READEND ; end routine of readding disk
	sub di, 0x1

	mov ah, 0x02
	mov al, 0x1
	mov ch, byte[TRACKNUMBER]
	mov cl, byte[SECTORNUMBER]
	mov dh, byte[HEADNUMBER]
	mov dl, 0x00
	int 0x13 ; interrupt 13(DISK IO)
	jc HANDLEDISKERROR

	add si, 0x0020 ; 512byte(0x0200) read(end)
	mov es, si ; set Data Segment(ES)

	mov al, byte[SECTORNUMBER] ; set sector number
	add al, 0x01 ; add one to sector number
	mov byte[SECTORNUMBER], al
	cmp al, 19
	jl READDATA ; loop until 19

	xor byte[HEADNUMBER], 0x01 ; toggle first bit
	mov byte[SECTORNUMBER], 0x01 ; sector number set 0x01

	cmp byte[HEADNUMBER], 0x00
	jne READDATA ; if head number is not 0, goto READDATA

	add byte[TRACKNUMBER], 0x01
	jmp READDATA

READEND: ;; print END MESSAGE
	push LOADINGCOMPLETEMESSAGE
	push 1
	push 20
	call PRINTMESSAGE
	add sp, 6

	jmp 0x1000:0x0000 ; execute virtual os image

;;;;;;;;;;
; FUNCTION CODE AREA
;;;;;;;;;;
HANDLEDISKERROR:
	push DISKERRORMESSAGE
	push 1
	push 20
	call PRINTMESSAGE

	jmp $ ; loop this line

;;print a string(message)
; PARAM : position x, position y, string
PRINTMESSAGE:
	push bp ; backup base pointer
	mov bp, sp ; load stack pointer

	;; backup 6 registers
	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax ; set Data Segment(ES) to 0xB800

	;; get y position
	mov ax, word[bp + 6] ; assign ax = second parameter
	mov si, 160 ; 2 * 80 ( one line )
	mul si
	mov di, ax ; set ax to di

	;; get x position
	mov ax, word[bp + 4] ; assign ax = first parameter
	mov si, 2 ; 2 ( one character )
	mul si
	add di, ax ; add ax to di

	mov si, word[bp + 8] ; parameter 3

.MESSAGELOOP:
	mov cl, byte[si] ; move data to EAX LOWER bit
	cmp cl, 0
	je .MESSAGEEND

	mov byte[es:di], cl

	add si, 1 ; add string index 1
	add di, 2

	jmp .MESSAGELOOP

.MESSAGEEND:
	;; restore 6 registers
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret ; return to code(callee)

;;;;;;;;;;
; DATA SECTION
;;;;;;;;;;

;;string
MESSAGE1: db 'HELLO THIS IS OS', 0 ; bootloader message
DISKERRORMESSAGE:	db 'DISK ERROR OCCURRED', 0
IMAGELOADINGMESSAGE:	db 'OS IMAGE LOADING...', 0
LOADINGCOMPLETEMESSAGE	db 'COMPLETE', 0

;;integer
SECTORNUMBER:		db 0x02 ; sector number
HEADNUMBER:		db 0x00 ; head number
TRACKNUMBER:		db 0x00 ; track number

times 510 - ($ - $$) db 0x00 ; fill 0x00 from $(current line) to 510

db 0x55
db 0xAA

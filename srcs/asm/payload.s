section .text

global hash
global payload

extern printf

payload:
	;; save cpu state
	push rdi
	;; do your evil thing
	lea rdi, [rel msg]			; pointer to msg (char* [])
	call printf

	;; restore cpu state
	pop rdi
	call hash
	
	;; jump to main
	mov rax, 0x1111111111111111		; set rax back to normal
	jmp rax					; jump to it

hash:
	enter 0, 0
	push rdi
	lea rdi, [rel msg2]
	call printf
	pop rdi
	leave
	ret
	

section .data
	msg		db '....WOODY....', 0xa, 0
	msg2		db '....OUAI....', 0xa, 0

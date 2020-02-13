section .text

extern printf

global main

main:
	;; save cpu state
	push rdi
	;; do your evil thing
	lea rdi, [rel msg]			; pointer to msg (char* [])
	call printf

	;; restore cpu state
	pop rdi

	;; jump to main
	;mov rax, 0x1111111111111111		; set rax back to normal
	;jmp rax					; jump to it
	;leave

section .data
	msg		db '....WOODY....', 0xa, 0

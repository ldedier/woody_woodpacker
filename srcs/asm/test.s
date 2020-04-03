section .text
    global _start


_start:
; OFFSET
; 
	call _pc
_pc:
	pop r14

	sub r14, 0x5


   push rax
   push rdi
   push rsi
   push rdx


   mov rax, 1             
   mov rdi, 1             
   lea rsi, [rel msg]     
   mov rdx, msg_end - msg 
   syscall

   ;; restore cpu state
   pop rdx
   pop rsi
   pop rdi
   pop rax

   mov rax, 0x1111111111111111
   push rdi

   add rax, r14
   jmp rax

align 8
   msg      db '...WOODY...', 10, 0
   msg_end  db 0x0

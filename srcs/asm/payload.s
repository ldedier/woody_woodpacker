section .text
    global payload
    global hash

payload:

   call _rip
_rip:
   pop r14
   ;; save cpu state
   push rax
   push rdi
   push rsi
   push rdx
   push r14

   ;; do your evil thing

   mov rax, 1             ; syscall number
   mov rdi, 1             ; fd = 1(stdout)
   lea rsi, [rel msg]     ; pointer to msg (char* [])
   mov rdx, msg_end - msg ; size
   syscall                ;
   
;   call hash

   ;; restore cpu state
   pop r14
   pop rdx
   pop rsi
   pop rdi
   pop rax

   ; set rax back to normal
   ; jump to main
   mov r15, 0x1111111111111111
   mov rax, 0x2222222222222222
   cmp rax, 0
   jz nopie
   mov rax, r14
nopie:
   add rax, r15
   jmp rax      ; jump to it

hash:
   enter 0, 0
   leave
   ret

align 8
   msg      db '...WOODY...', 0xa , 0
   msg_end  db 0x0

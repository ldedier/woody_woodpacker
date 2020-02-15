section .text
    global payload
    global hash

payload:
   ;; save cpu state
   push rax
   push rdi
   push rsi
   push rdx

   ;; do your evil thing


   mov rax, 1             ; syscall number
   mov rdi, 1             ; fd = 1(stdout)
   lea rsi, [rel msg]     ; pointer to msg (char* [])
   mov rdx, msg_end - msg ; size
   syscall                ;
   
;   call hash

   ;; restore cpu state
   pop rdx
   pop rsi
   pop rdi
   pop rax

   ;; jump to main
   mov rax, 0x1111111111111111    ; set rax back to normal
   jmp rax                        ; jump to it

;hash:
;   enter 0, 0
;   leave
;   ret

align 8
   msg      db '...WOODY...', 0xa , 0
   msg_end  db 0x0

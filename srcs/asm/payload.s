section .text
    global payload
    global hash

payload:

   call _rip
_rip:
   pop r14
   ;; save cpu state
   push rdi
   push rsi
   push rdx

   ;; do your evil thing

   mov rax, 1             ; syscall number
   mov rdi, 1             ; fd = 1(stdout)
   lea rsi, [rel msg]     ; pointer to msg (char* [])
   mov rdx, msg_end - msg ; size
   syscall                
   ; set rax back to normal
   ; jump to main
   mov r15, 0x1111111111111111 ; get initial entry point (relative or absolutei to program base address)
   mov rax, 0x2222222222222222 ; 0 if PIE is desactivated
   mov rdi, 0x3333333333333333 ; address of text section (always relative)
   cmp rax, 0
   jz nopie
   mov rax, r14
   add rdi, r14
nopie:
   add rax, r15

;   call hash

   mov rsi, 0x4444444444444444 ; size of text section
   mov rdx, 0x5555555555555555 ; key of RC4
   call hash

   ;; restore cpu state
   pop rdx
   pop rsi
   pop rdi

   jmp rax ; jump to it

fill_swap_buffer:
   enter 0, 0
   xor rcx, rcx
fill_swap_loop:
   mov byte[rdi + rcx], cl
   inc rcx
   cmp rcx, 0xff
   jl fill_swap_loop
   leave
   ret

   ; rdi: address to hash
   ; rsi: size to hash
   ; rdx: key to hash

hash:
   enter 0xff, 0
   sub rsp, 0x30
   mov qword[rbp - 0x10f], rdi
   lea rdi, [rbp - 0xff] ; S
   mov r10, 0 ; i
   mov r11, 0 ; j
   call fill_swap_buffer
   xor rcx, rcx
   mov rdi, [rbp - 0x10f]
hash_loop:
   add byte[rdi + rcx], 128
   inc rcx
   cmp rcx, rsi
   jl hash_loop
   leave
   ret

align 8
   msg      db '...WOODY...', 0xa , 0
   msg_end  db 0x0

section .text
    global _start

_start:
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
   syscall                ; ( SYS_write = rax(1), fd = rdi(1), buff = rsi(char *msg), size = rdx(len(msg)))

   ;; restore cpu state
   pop rdx
   pop rsi
   pop rdi
   pop rax

   ;; jump to main
   mov rax, 0x1111111111111111    ; set rax back to normal
   jmp rax                        ; jump to it

align 8
   msg      db 'Hello world', 0xa, 0
   msg_end  db 0x0

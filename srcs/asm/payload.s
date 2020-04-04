segment .text
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
   push r15
   push r14

   ;; do your evil thing

   mov rax, 1             ; syscall number
   mov rdi, 1             ; fd = 1(stdout)
   lea rsi, [rel msg]     ; pointer to msg (char* [])
   mov rdx, msg_end - msg - 1 ; size
   syscall

   ; set rax back to normal
   ; jump to main
   mov r15, 0x1111111111111111 ; get initial entry point (relative or absolute to program base address)
   mov rax, 0x2222222222222222 ; 0 if PIE is desactivated
   mov rdi, 0x3333333333333333 ; address of text section (relative or absolute to program base address)
   cmp rax, 0
   jz nopie
   mov rax, r14
   add rdi, r14		       ; absolute address of text section
nopie:
   add rax, r15			; add to relative entry point the new entry point address (so rax contains absolute old entry point)

   lea rsi, [rel key] ; key of RC4
   mov rdx, 0x4444444444444444  ; size of text section
   push rax
   call hash

   ;; restore cpu state
   pop rax

   pop r14
   pop r15
   pop rdx
   pop rsi
   pop rdi

   jmp rax ; jump to it

; rdi: S
; rsi: RC4 key

fill_swap_buffer:
   enter 0x20, 0
   push rbx
   mov [rbp - 0x8], rdi
   mov [rbp - 0x10], rsi
   mov rdi, rsi
   call payload_strlen
   mov [rbp - 0x18], rax
  ; mov rsi, [rbp - 0x10]
   mov rdi, [rbp - 0x8]
   xor rcx, rcx
fill_swap_loop_identity:
   mov byte[rdi + rcx], cl
   inc rcx
   cmp rcx, 0xff
   jle fill_swap_loop_identity
   
   xor rcx, rcx
   mov r11, 0

;jmp fill_swap_end

fill_swap_loop:

   add r11b, byte[rdi + rcx]

   mov rax, rcx
   xor rdx, rdx
   div qword [rbp - 0x18] ; rdx = i mod keylength

   add r11b, byte[rsi + rdx]
   and r11, 0xff

   mov bl, byte[rdi + rcx]
   xchg byte[rdi + r11], bl
   mov byte[rdi + rcx], bl

   inc rcx
   cmp rcx, 0xff
   jle fill_swap_loop

fill_swap_end:
   pop rbx
   leave
   ret

payload_strlen:
   xor rcx, rcx
   not rcx
   xor al, al
   cld
   repne scasb
   not rcx
   dec rcx
   mov rax, rcx
   ret

   ; rdi: address to hash
   ; rsi: key to hash
   ; rdx: size to hash

hash:
   enter 0x110, 0
   push r12
   mov qword[rbp - 0x108], rdi
   mov qword[rbp - 0x110], rdx
   lea rdi, [rbp - 0x100] ; S
   mov r10, 0 ; i
   mov r11, 0 ; j
   call fill_swap_buffer
   mov rsi, qword[rbp - 0x108] ; rsi becomes address to hash
   xor rcx, rcx
   xor r10, r10 ; i
   xor r11, r11 ; j
   xor r12, r12
   mov rdx, qword[rbp - 0x110] ; rdx still size to hash
			  ; rdi : BUFFER S


hash_loop:

   cmp rcx, rdx
   jge hash_loop_end

   inc r10b
   add r11b, byte[rdi + r10]
   mov al, byte[rdi + r10]
   xchg al, byte[rdi + r11]
   mov byte[rdi + r10], al
   mov r12b, byte[rdi + r10]
   add r12b, byte[rdi + r11]
   mov r12b, byte[rdi + r12]
   xor byte[rsi + rcx], r12b
   inc rcx
   jmp hash_loop

hash_loop_end:
   pop r12
   leave
   ret

align 8
   msg      db '...WOODY...', 0xa , 0
   msg_end  db 0x0
   key      db '___TO_REMPLACE_KEY___' , 0

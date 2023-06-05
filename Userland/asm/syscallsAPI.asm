GLOBAL sys_read_asm
GLOBAL sys_write_asm
GLOBAL sys_print_asm
GLOBAL sys_ticker_asm
GLOBAL sys_memcpy_asm
GLOBAL sys_inb_asm
GLOBAL sys_out_asm
GLOBAL sys_accessRTC_asm
GLOBAL sys_malloc_asm
GLOBAL sys_free_asm
GLOBAL sys_m_dump_asm
GLOBAL sys_p_create_asm
GLOBAL sys_processDisplay_asm
GLOBAL sys_getPID_asm
GLOBAL sys_block_asm
GLOBAL sys_unblock_asm
GLOBAL sys_kill_asm
GLOBAL sys_sem_asm
GLOBAL sys_yield_asm
GLOBAL sys_waitpid_asm
GLOBAL sys_priority_asm
GLOBAL sys_toggle_asm
GLOBAL sys_open_pipe_asm
GLOBAL sys_close_pipe_asm
GLOBAL sys_print_pipes_asm

READ equ 0
WRITE equ 1
BEEP_INB equ 2
BEEP_OUTB equ 3
PRINT equ 4
TICKER equ 5
MEMCPY equ 6
RTC equ 7
MALLOC equ 8
FREE equ 9
M_DUMP equ 10
P_CREATE equ 11
P_DISPLAY equ 12
PID equ 13
BLOCK equ 14
UNBLOCK equ 15
KILL equ 16
SEM equ 17
YIELD equ 18
WAITPID equ 19
PRIORITY equ 20
TOGGLE equ 21
OPEN_PIPE equ 22
CLOSE_PIPE equ 23
PRINT_PIPES equ 24

SECTION .text

sys_read_asm: ; int sys_read(int fd, char *buf, int count)
    push rbp
    mov rbp, rsp

    mov r8,rcx  ;Passing arguments from C to syscall parameters
    mov rcx,rdx
    mov rdx,rsi
    mov rsi,rdi
    mov rdi,READ
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_write_asm: ; int sys_write(int fd, char *buf, int count)
    push rbp
    mov rbp, rsp

    mov r8,rcx ;Passing arguments from C to syscall parameters
    mov rcx,rdx
    mov rdx,rsi
    mov rsi,rdi
    mov rdi,WRITE
    int 80h

    mov rsp,rbp
    pop rbp
    ret
    
sys_print_asm: ; int sys_print(int fd, int length, int coor)
    push rbp
    mov rbp, rsp

    mov r8,rcx ;Passing arguments from C to syscall parameters
    mov rcx,rdx
    mov rdx,rsi
    mov rsi,rdi
    mov rdi,PRINT
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_ticker_asm: ; int sys_ticker(int fd, int length)
    push rbp
    mov rbp, rsp

    mov rdx,rsi ;Passing arguments from C to syscall parameters
    mov rsi, rdi
    mov rdi, TICKER
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_memcpy_asm: ; int sys_memcpy(char *buf, int count)
    push rbp
    mov rbp, rsp

    mov rdx, rsi ;Passing arguments from C to syscall parameters
    mov rsi, rdi
    mov rdi, MEMCPY
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_out_asm: ; int sys_out(int port, int value)
    push rbp
    mov rbp, rsp

    mov rdx, rsi ;Passing arguments from C to syscall parameters
    mov rsi, rdi
    mov rdi, BEEP_OUTB

    mov rsp, rbp
    pop rbp
    ret

sys_inb_asm: ; int sys_inb(int port)
    push rbp
    mov rbp, rsp

    mov rsi, rdi ;Passing arguments from C to syscall parameters
    mov rdi, BEEP_INB
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_accessRTC_asm: ; int sys_accessRTC(int fd)
    push rbp
    mov rbp, rsp

    mov rsi, rdi ;Passing arguments from C to syscall parameters
    mov rdi, RTC
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_malloc_asm:        ; void * sys_malloc(size_t size)
    push rbp
    mov rbp, rsp

    mov rsi, rdi ;Passing arguments from C to syscall parameters
    mov rdi, MALLOC
    int 80h

    mov rsp,rbp
    pop rbp
    ret    

sys_free_asm:           ; sys_free_asm(void * ptr)
    push rbp
    mov rbp, rsp

    mov rsi, rdi ;Passing arguments from C to syscall parameters
    mov rdi, FREE
    int 80h

    mov rsp,rbp
    pop rbp
    ret

sys_m_dump_asm:     ;sys_m_dump_asm();
    push rbp
    mov rbp, rsp

    mov rdi, M_DUMP
    int 80h

    mov rsp, rbp
    pop rbp
    ret        

sys_p_create_asm:       ; sys_p_create(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
    push rbp
    mov rbp, rsp

    mov r9,r8
    mov r8,rcx 
    mov rcx,rdx
    mov rdx,rsi
    mov rsi, rdi ;Passing arguments from C to syscall parameters
    mov rdi, P_CREATE
    int 80h

    mov rsp,rbp
    pop rbp
    ret    

sys_processDisplay_asm:     ;sys_processDisplay_asm();
    push rbp
    mov rbp, rsp

    mov rdi, P_DISPLAY
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_getPID_asm:             ;sys_getPID_asm()
    push rbp
    mov rbp, rsp

    mov rdi, PID
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_block_asm:              ;sys_block_asm(uint64_t)
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, BLOCK
    int 80h

    mov rsp, rbp
    pop rbp
    ret    


sys_unblock_asm:              ;sys_unblock_asm(uint64_t)
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, UNBLOCK
    int 80h

    mov rsp, rbp
    pop rbp
    ret


sys_kill_asm:              ;sys_kill_asm(uint64_t)
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, KILL
    int 80h

    mov rsp, rbp
    pop rbp
    ret                

sys_sem_asm:              ;sys_sem_asm(uint32_t id, char * name, uint32_t initValue)
    push rbp
    mov rbp, rsp

    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, SEM
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_yield_asm:              ;sys_yield_asm()
    push rbp
    mov rbp, rsp

    mov rdi, YIELD
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_waitpid_asm:              ;sys_waitpid_asm(uint64_t pid)
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, WAITPID
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_priority_asm:          ;sys_priority_asm(uint64_t pid, int priority)
    push rbp
    mov rbp, rsp

    mov rdx, rsi ;Passing arguments from C to syscall parameters
    mov rsi, rdi
    mov rdi, PRIORITY

    mov rsp, rbp
    pop rbp
    ret

sys_toggle_asm:          ;sys_toggle_asm(uint64_t pid)
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, TOGGLE
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sys_open_pipe_asm:
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, OPEN_PIPE
    int 80h

    mov rsp, rbp
    pop rbp
    ret


sys_close_pipe_asm:
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, CLOSE_PIPE
    int 80h

    mov rsp, rbp
    pop rbp
    ret 

sys_print_pipes_asm:     
    push rbp
    mov rbp, rsp

    mov rdi, PRINT_PIPES
    int 80h

    mov rsp, rbp
    pop rbp
    ret 


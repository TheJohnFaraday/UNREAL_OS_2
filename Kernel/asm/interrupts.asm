GLOBAL _cli
GLOBAL _sti
GLOBAL _hlt

GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _exception8Handler

GLOBAL timerRoutine
GLOBAL keyboardRoutine
GLOBAL systemCallsRoutine
GLOBAL getRegisters
GLOBAL registerBuffer
GLOBAL stackAddrAfterPushState

EXTERN loader
EXTERN irqDispatcher
EXTERN timer_handler
EXTERN exceptionDispatcher
EXTERN fetchKeyboardEvent
EXTERN syscallHandler
EXTERN scheduler

EXTERN sys_accessRTC
EXTERN sys_read
EXTERN sys_beepInb
EXTERN sys_beepOutb
EXTERN sys_write
EXTERN sys_memcpy
EXTERN sys_print
EXTERN sys_ticker 
EXTERN sys_allocMem
EXTERN sys_free
EXTERN sys_m_dump
EXTERN sys_p_create
EXTERN sys_processDisplay
EXTERN sys_getPID
EXTERN sys_block
EXTERN sys_unblock
EXTERN sys_kill
EXTERN sys_open_pipe
EXTERN sys_read_pipe
EXTERN sys_write_pipe
EXTERN sys_close_pipe
EXTERN sys_print_pipes

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
OPEN_PIPE 22
READ_PIPE 23
WRITE_PIPE 24
CLOSE_PIPE 25 
PRINT_PIPES 26

SECTION .text

_cli:
	cli
	ret


_sti:
	sti
	ret

_hlt:
	sti
	hlt
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out 0A1h, al
    pop rbp
    retn
	
%macro pushState 0
	push rax
	pushStateSys
	mov [stackAddrAfterPushState], rsp 
%endmacro 	

%macro timerPush 0
	push rax      
    push rbx      
    push rcx      
    push rdx      
    push rbp      
    push rdi      
    push rsi      
    push r8       
    push r9       
    push r10      
    push r12     
    push r11     
    push r13     
    push r14     
    push r15     
    push fs
    push gs
%endmacro

%macro timerPop 0
	pop gs
    pop fs
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro pushStateSys 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rsp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateSys 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rsp
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro popState 0
	popStateSys
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	;override the iretq return address and goes to the start of the userland _loader.c archive
	;this archive calls the main function of the userland and the main calls the shell
	push rax 				;push rax to conserve it's data
	mov rax, loader 			;address of _start in _loader.c
	mov [rsp+8], rax 			;override the iretq return address
	pop rax 				;restore rax
	iretq
	iretq
%endmacro

%macro endInterrupt 0
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
%endmacro

timerRoutine: ;Timer tick handler
    timerPush

	call timer_handler

	mov rdi,rsp
	call scheduler
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
	endInterrupt

	timerPop
	iretq

keyboardRoutine: ;Keyboard Interrupt handler
    pushState

	call fetchKeyboardEvent

	; signal pic EOI (End of Interrupt)
	endInterrupt

	popState
	iretq

systemCallsRoutine:  ;Arguments received depending on the system call
	pushStateSys
	mov rbp, rsp

	mov rbx, rdi				;getting the id for syscall
	
	mov rdi, rsi				;rearranging the args for the syscall
	mov rsi, rdx
	mov rdx, rcx			
	mov rcx, r8
	mov r8, r9
 
	;calling the syscall
	;Basic Syscalls
	cmp rbx, WRITE
	je .write_handler
	cmp rbx, READ
	je .read_handler
	cmp rbx, BEEP_INB
	je .beep_inb_handler
	cmp rbx, BEEP_OUTB
	je .beep_outb_handler
	cmp rbx, PRINT
	je .print_handler
	cmp rbx, TICKER
	je .ticker_handler
	cmp rbx, MEMCPY
	je .memcpy_handler
	cmp rbx, RTC
	je .rtc_handler

	;Memory Syscalls
	cmp rbx, MALLOC
	je .malloc_handler
	cmp rbx, FREE
	je .free_handler
	cmp rbx, M_DUMP
	je .m_dump_handler

	;Process Syscalls
	cmp rbx, P_CREATE
	je .process_create_handler
	cmp rbx, P_DISPLAY
	je .process_display_handler
	cmp rbx, PID
	je .getPID_handler
	cmp rbx, BLOCK
	je .block_handler
	cmp rbx, UNBLOCK
	je .unblock_handler
	cmp rbx, KILL
	je .kill_handler

	;Pipes Syscalls
	cmp rbx, OPEN_PIPE 
	je .pipe_open_handler
	cmp rbx, READ_PIPE 
	je .pipe_read_handler
	cmp rbx, WRITE_PIPE 
	je .pipe_write_handler
	cmp rbx, CLOSE_PIPE 
	je .pipe_close_handler
	cmp rbx, PRINT_PIPES 
	je .pipe_print_handler

.pipe_open_handler
	call sys_open_pipe
	jmp .end_sys

.pipe_read_handler
	call sys_read_pipe
	jmp .end_sys

.pipe_write_handler
	call sys_write_pipe
	jmp .end_sys

.pipe_close_handler
	call sys_close_pipe
	jmp .end_sys

.pipe_print_handler
	call sys_print_pipes
	jmp .end_sys

.end_sys:
	mov rsp,rbp
	popStateSys
	iretq

.write_handler:
	call sys_write
	jmp .end_sys

.read_handler:
	call sys_read
	jmp .end_sys

.beep_inb_handler:
	call sys_beepInb
	jmp .end_sys

.beep_outb_handler:
	call sys_beepOutb
	jmp .end_sys

.print_handler:
	call sys_print
	jmp .end_sys

.ticker_handler:
	call sys_ticker
	jmp .end_sys

.memcpy_handler:
	call sys_memcpy
	jmp .end_sys

.rtc_handler:
    call sys_accessRTC
	jmp .end_sys	

.malloc_handler:
	call sys_allocMem
	jmp .end_sys	

.free_handler:
	call sys_free
	jmp .end_sys

.m_dump_handler:
	call sys_m_dump
	jmp .end_sys

.process_create_handler:
	call sys_p_create
	jmp .end_sys	

.process_display_handler:
	call sys_processDisplay
	jmp .end_sys

.getPID_handler:
	call sys_getPID
	jmp .end_sys	

.block_handler:
	call sys_block
	jmp .end_sys

.unblock_handler:
	call sys_unblock
	jmp .end_sys

.kill_handler:
	call sys_kill
	jmp .end_sys		

_exception0Handler:
	exceptionHandler 0

_exception6Handler:
	exceptionHandler 6
	
_exception8Handler:
	exceptionHandler 8

getRegisters: ;Copies registers to a buffer in memory
	mov QWORD[registerBuffer], rax
	mov QWORD[registerBuffer + 8], rbx
	mov QWORD[registerBuffer + 16], rcx
	mov QWORD[registerBuffer + 24], rdx
	mov QWORD[registerBuffer + 32], rbp
	mov QWORD[registerBuffer + 40], rsp
	mov QWORD[registerBuffer + 48], rsi
	mov QWORD[registerBuffer + 56], rdi
	mov QWORD[registerBuffer + 64], r8
	mov QWORD[registerBuffer + 72], r9
	mov QWORD[registerBuffer + 80], r10
	mov QWORD[registerBuffer + 88], r11
	mov QWORD[registerBuffer + 96], r12
	mov QWORD[registerBuffer + 104], r13
	mov QWORD[registerBuffer + 112], r14
	mov QWORD[registerBuffer + 120], r15
	
	;Return address of buffer in .bss
	mov rax, registerBuffer
	ret


SECTION .bss
	stackAddrAfterPushState resb 8
	aux resq 1
	registerBuffer resb 136

/* Real Mode Hello World */
/*
.code16
.global start
start:
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw $0x7d00, %ax
    movw %ax, %sp # setting stack pointer to 0x7d00
    pushw $13 # pushing the size to print into stack
    pushw $message # pushing the address of message into stack
    callw displayStr # calling the display function
loop:
    jmp loop

message:
    .string "Hello, World!\n\0"

displayStr:
    pushw %bp
    movw 4(%esp), %ax
    movw %ax, %bp
    movw 6(%esp), %cx
    movw $0x1301, %ax
    movw $0x000c, %bx
    movw $0x0000, %dx
    int $0x10
    popw %bp
    ret
*/
/* Protected Mode Hello World */
.code16
.global start
start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	cli
	inb $0x92,%al
	orb $0x02,%al
	outb %al,$0x92 #open A20
	data32 addr32 lgdt gdtDesc
	movl %cr0,%eax
	orb  $0x1,%al
	movl %eax,%cr0  #cr0's PE =1
	data32 ljmp $0x08, $start32 #into protected mode
.code32
start32:
	movw $0x10, %ax # setting data segment selector
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %ss
	movw $0x18, %ax
	movw %ax,%gs
	movl $0x8000, %eax # setting esp
	movl %eax, %esp
     	jmp bootMain # jump to bootMain in boot.c
	pushl $13 # pushing the size to print into stack
	pushl $message # pushing the address of message into stack
	calll displayStr # calling the display function
loop32:
	jmp loop32

message:
	.string "Hello, World!\n\0"

displayStr:
	movl 4(%esp), %ebx
	movl 8(%esp), %ecx
	movl $((80*5+0)*2), %edi
	movb $0x0c, %ah
nextChar:
	movb (%ebx), %al
	movw %ax, %gs:(%edi)
	addl $2, %edi
	incl %ebx
	loopnz nextChar # loopnz decrease ecx by 1
	ret

.p2align 2
gdt: 
	.word 0,0
	.byte 0,0,0,0

	.word 0xffff,0
	.byte 0,0x9a,0xcf,0
	
	.word 0xffff,0
	.byte 0,0x92,0xcf,0
	
	.word 0xffff,0x8000
	.byte 0x0b,0x92,0xcf,0
gdtDesc: 
	.word (gdtDesc-gdt-1)#gdtDesc definition here
	.long gdt

/* Protected Mode Loading Hello World APP */
/*.code16

.global start
start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	#TODO: Protected Mode Here

.code32
start32:
	movw $0x10, %ax # setting data segment selector
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %ss
	movw $0x18, %ax # setting graphics data segment selector
	movw %ax, %gs
	
	movl $0x8000, %eax # setting esp
	movl %eax, %esp
	jmp bootMain # jump to bootMain in boot.c

.p2align 2
gdt: 
	#GDT definition here

gdtDesc: 
	#gdtDesc definition here
	*/

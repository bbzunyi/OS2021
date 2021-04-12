#include "x86.h"
#include "device.h"

extern int displayRow;
extern int displayCol;

extern uint32_t keyBuffer[MAX_KEYBUFFER_SIZE];
extern int bufferHead;
extern int bufferTail;


void GProtectFaultHandle(struct TrapFrame *tf);

void KeyboardHandle(struct TrapFrame *tf);

void syscallHandle(struct TrapFrame *tf);
void syscallWrite(struct TrapFrame *tf);
void syscallPrint(struct TrapFrame *tf);
void syscallRead(struct TrapFrame *tf);
void syscallGetChar(struct TrapFrame *tf);
void syscallGetStr(struct TrapFrame *tf);


void irqHandle(struct TrapFrame *tf) { // pointer tf = esp
	/*
	 * 中断处理程序
	 */
	/* Reassign segment register */
	asm volatile("movw %%ax, %%ds"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%es"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%fs"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%gs"::"a"(KSEL(SEG_KDATA)));
	switch(tf->irq) {
		// TODO: 填好中断处理程序的调用
		case -1:
			break;
		case 0xd:
			GProtectFaultHandle(tf);
			break;
		case 0x21:
			KeyboardHandle(tf);
			break;
		case 0x80:
			syscallHandle(tf);
			break;
		default:assert(0);
	}
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}

void KeyboardHandle(struct TrapFrame *tf){
	uint32_t code = getKeyCode();
	char character=getChar(code);
	uint16_t data;
	int pos;
	if(code == 0xe){ // 退格符
		if(displayCol>0){
		displayCol--;
		data = 0 | (0x0c<<8);
		pos =(80*displayRow + displayCol)*2;
		asm volatile("movw %0,(%1)"::"r"(data),"r"(pos+0xb8000));
		if(bufferTail>0)
		bufferTail--;// TODO: 要求只能退格用户键盘输入的字符串，且最多退到当行行首
		}
	}
	else if(code == 0x1c){ // 回车符
		displayRow+=1;
		displayCol=0;
		if(displayRow>=25){
			displayRow=24;			
			scrollScreen();
		}
		keyBuffer[++bufferTail]=character;
	}
	else if(code < 0x81){ // 正常字符
		if(code==0x2a||code == 0x36||code == 0x38||code == 0x3a||code == 0x1d){
			keyBuffer[bufferHead]=code;
		}
		else{
		if(keyBuffer[bufferHead]==0x2a||keyBuffer[bufferHead]==0x36){
			keyBuffer[bufferHead]=0;
			character-=0x20;
		}
		data = character | (0x0c<<8);
		pos =(80*displayRow + displayCol)*2;
		keyBuffer[++bufferTail]=character;
		asm volatile("movw %0,(%1)"::"r"(data),"r"(pos+0xb8000));
		displayCol=(displayCol+1)%80;
			if(displayCol==0){
				displayRow++;
				if(displayRow>=25){
					displayRow=24;
					scrollScreen();
				}
			}
		}//TODO: 注意输入的大小写的实现、不可打印字符的处理
	
	}

	updateCursor(displayRow, displayCol);
}

void syscallHandle(struct TrapFrame *tf) {
	switch(tf->eax) { // syscall number
		case 0:
			syscallWrite(tf);
			break; // for SYS_WRITE
		case 1:
			syscallRead(tf);
			break; // for SYS_READ
		default:break;
	}
}

void syscallWrite(struct TrapFrame *tf) {
	switch(tf->ecx) { // file descriptor
		case 0:
			syscallPrint(tf);
			break; // for STD_OUT
		default:break;
	}
}

void syscallPrint(struct TrapFrame *tf) {
	int sel = USEL(SEG_UDATA); //TODO: segment selector for user data, need further modification
	char *str = (char*)tf->edx;
	int size = tf->ebx;
	int i = 0;
	int pos = 0;
	char character = 0;
	uint16_t data = 0;
	asm volatile("movw %0, %%es"::"m"(sel));
	for (i = 0; i < size; i++) {
		asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i));
		// TODO: 完成光标的维护和打印到显存
		if(character=='\n'){
			displayRow+=1;
			displayCol=0;
			if(displayRow>=25){
				displayRow=24;
				scrollScreen();
			}
		}
		else{
			data = character | (0x0c<<8);
			pos =(80*displayRow + displayCol)*2;
			asm volatile("movw %0,(%1)"::"r"(data),"r"(pos+0xb8000));
			displayCol=(displayCol+1)%80;

			if(displayCol==0){
				displayRow++;
				if(displayRow>=25){
					displayRow=24;
					scrollScreen();
				}
			}
		}
	}
	
	updateCursor(displayRow, displayCol);
}

void syscallRead(struct TrapFrame *tf){
	switch(tf->ecx){ //file descriptor
		case 0:
			syscallGetChar(tf);
			break; // for STD_IN
		case 1:
			syscallGetStr(tf);
			break; // for STD_STR
		default:break;
	}
}

void syscallGetChar(struct TrapFrame *tf){
	bufferHead = 0, bufferTail = 0;
	keyBuffer[0]=0;
	while(keyBuffer[bufferTail]!='\n')
		enableInterrupt();
	int charactor = keyBuffer[1];
	tf->eax = charactor;
	disableInterrupt();
}

void syscallGetStr(struct TrapFrame *tf){
	bufferHead = 0;
	 bufferTail = 0;
	keyBuffer[bufferHead] = 0;
	int sel =  USEL(SEG_UDATA);
	asm volatile("movw %0, %%es"::"m"(sel));
	char charactor = 0;
	char* str = (char*)tf->edx;
	while (keyBuffer[bufferTail] != '\n')
		 enableInterrupt();
		
	for (int i = 0;i < 100;i++){
		charactor = (char)(keyBuffer[i+1]);
		if(charactor=='\n')break;
		asm volatile("movb %0, %%es:(%1)"::"r"(charactor),"r"(str+i));
	}
	disableInterrupt();// TODO: 自由实现
}

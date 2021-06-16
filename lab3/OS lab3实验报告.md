# OS lab3实验报告

|  姓名  |   学号    |       邮箱        |        院系        |
| :----: | :-------: | :---------------: | :----------------: |
| 张洋彬 | 191220169 | 1016466918@qq.com | 计算机科学与技术系 |

[TOC]

## 一、实验目的

- 实现时间中断处理函数`timerHandle(struct StackFrame*sf)`
- 实现`fork`、`exit`、`sleep`库函数即处理例程

## 二、实验预备知识
1、进程控制块的结构
![截屏2021-04-29 上午11.17.50](/Users/mac/Desktop/截屏2021-04-29 上午11.17.50.png)
2、各个进程的段基址
![截屏2021-04-29 上午11.20.14](/Users/mac/Library/Application Support/typora-user-images/截屏2021-04-29 上午11.20.14.png)

![截屏2021-04-29 上午11.20.35](/Users/mac/Library/Application Support/typora-user-images/截屏2021-04-29 上午11.20.35.png)

## 三、实验过程

#### 3.1 完成库函数

​	直接系统调用即可，如代码框所示。

```c
pid_t fork() {
	return syscall(SYS_FORK, 0, 0, 0, 0, 0);
}


int sleep(uint32_t time) {
	return syscall(SYS_SLEEP,time,0,0,0,0);
}

int exit() {
	return syscall(SYS_EXIT,0,0,0,0,0);
}
```



#### 3.2 时间中断处理

1. 遍历pcb，将状态为`STATE_BLOCKED`的进程的**sleepTime**减一，如果进程的**sleepTime**变为0，重新设为`STATE_RUNNABLE`
2. 将当前进程的timeCount加一，如果时间片用完（`timeCount>=MAX_TIME_COUNT`）且有其它状态为`STATE_RUNNABLE`的进程，切换;否则切换到内核IDLE进程，等待中断的到来,并将切换到的进程的状态设为
`STATE_RUNNING`，伪代码实现如下：
```c
	for(进程状态为blocked的进程块) do:
		sleepTime--;
		if(sleepTime<=0)
		把状态改为runnable
	
	将当前正在运行的进程的timecount++；
	如果时间片用完就将当前进程的状态置为runnable
	
	if(pcb[current].state!=STATE_RUNNING){
		int i=(current+1)%MAX_PCB_NUM;
    for( ; i!=current ; i=(i+1)%MAX_PCB_NUM){
        if(pcb[i].state==STATE_RUNNABLE&&i!=0){
           break;
        	}
        }
    if(current==i)
        current=0;
    else
    {
        current=i;
     }
    pcb[current].timeCount=0;
    pcb[current].sleepTime=0;
    pcb[current].state=STATE_RUNNING;
		uint32_t tmpStackTop = pcb[current].stackTop; 
		pcb[current].stackTop = pcb[current].prevStackTop; 
		tss.esp0 = (uint32_t)&(pcb[current].stackTop); 
		asm volatile("movl %0, %%esp"::"m"(tmpStackTop)); // switch kernel stack 
		asm volatile("popl %gs"); 
		asm volatile("popl %fs"); 
		asm volatile("popl %es"); 
		asm volatile("popl %ds"); 
		asm volatile("popal"); 
		asm volatile("addl $8, %esp"); 
		asm volatile("iret");
	}
```
​	最后一步，在irqhandle里增加保存与恢复的内容，实现进程的切换。



#### 3.3 系统调用例程

##### 3.3.1 syscallFork

- 找到进程状态为`STATE_DEAD`的进程，这个位置用于创建子进程

- 把父进程的数据段和代码段复制给子进程

- 复制父进程的pcb给子进程

- 修改子进程中pcb的值

- 父进程返回子进程的pid，子进程返回0

```c
for (int i = 0; i < 0x100000; i++) {
			*(uint8_t *)(i + (index + 1) * 0x100000) = *(uint8_t *)(i + (current + 1) * 0x100000);
		}
```

##### 3.3.2 syscallSleep

​	将当前的进程的sleepTime设置为传入的参数，将当前进程的状态设置为STATE_BLOCKED，然后模拟时钟中断。

```c
void syscallSleep(struct StackFrame *sf) {
	pcb[current].sleepTime = sf->ecx;
	pcb[current].state = STATE_BLOCKED;
	asm volatile("int $0x20");
}
```



##### 3.3.3 syscallExit

​	将当前进程的状态设置为STATE_DEAD，然后模拟时钟中断进行进程切换

```c
void syscallExit(struct StackFrame *sf) {
	pcb[current].state = STATE_DEAD;
	asm volatile("int $0x20");
}
```



##### 3.4 中断嵌套

```c
		enableInterrupt();
		for (int i = 0; i < 0x100000; i++) {
			*(uint8_t *)(i + (index + 1) * 0x100000) = *(uint8_t *)(i + (current + 1) * 0x100000);
			if(i%0x10000==0)
			asm volatile("int $0x20");
		}
		disableInterrupt();
```

​	按照以上方式打开中断嵌套，运行结果如下![截屏2021-05-05 下午10.42.12](/Users/mac/Desktop/截屏2021-05-05 下午10.42.12.png)

## 四、实验结果

​	测试结果如下图所示：

![截屏2021-04-29 下午5.30.16](/Users/mac/Desktop/截屏2021-04-29 下午5.30.16.png)



## 五、感想与体会

​	本次实验难度和上一次相比差不多，但是写着特别舒服，因为实验的index介绍得很清楚，通过这次实验，更加清楚的理解了进程的切换，父子进程的创建等等问题，没有特别繁琐的过程，在这其中找到了实验的乐趣所在
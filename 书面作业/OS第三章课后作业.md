

# OS第三章课后作业

|  姓名  |   学号    |        院系        |       邮箱        |
| :----: | :-------: | :----------------: | :---------------: |
| 张洋彬 | 191220169 | 计算机科学与技术系 | 1016466918@qq.com |

## 一、问答题

##### 1、Does Peterson’s solution to the mutual-exclusion problem shown in Fig. 2-24 work when process scheduling is preemptive? How about when it is nonpreemptive?![截屏2021-05-11 下午7.59.31](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午7.59.31.png)

​	在抢占式调度中，上述算法能正常工作，但在非抢占式调度中，若执行上图的代码，若turn初始为0，P1首先运行时，因为interested[0]一直没有被置为0，所以会一直陷入循环中，永远不释放CPU。

​	

##### 2、In Sec. 2.3.4, a situation with a high-priority process, H, and a low-priority process, L, was described, which led to H looping forever. Does the same problem occur if round-robin scheduling is used instead of priority scheduling? Discuss.

![截屏2021-05-11 下午8.00.48](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午8.00.48.png)

​	使用RR算法的话，它可以正常工作，最终L会访问并离开临界区，但是通过优先级调度算法，L将永远得不到运行，使用轮转调度算法的话，L会定期得到时间片，就有机会去访问并离开临界区。

##### 3、Consider the following solution to the mutual-exclusion problem involving two processes P0 and P1. Assume that the variable turn is initialized to 0. Process P0’s code is presented below. For process P1, replace 0 by 1 in above code. Determine if the solution meets all the required conditions for a correct mutual-exclusion solution.![截屏2021-05-11 下午8.02.07](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午8.02.07.png)

​	该解决方案满足了互斥的需求，因为P0和P1不可能同时进入临界区。如果turn==0，那么P0就能进入临界区，P1永远进入不了临界区，turn==1的话就刚好相反。假设P0首先运行，P1产生某种东西放在缓冲区中，P0进入临界区时，他会发现缓冲区为空并阻塞，解决方案就希望P0和P1交替执行，但是仅用这个算法无法做到交替执行。



## 二、应用题

##### 3、 

![截屏2021-05-11 下午9.02.02](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午9.02.02.png)

​	总共有三种情况，分别如下：

1、P1中的值z=y+1一直延后执行（等到P2执行结束后），此时各个变量的值变化如下：

| x.value | y.value | z.value |
| :-----: | :-----: | :-----: |
|    1    |    1    |    5    |
|    6    |    4    |         |
|   10    |    9    |         |

x=10，y=9，z=5

2、先执行p1中的y=z+y，然后再执行p2中的z=z+x；

| x.value | y.value | z.value |
| :-----: | :-----: | :-----: |
|    1    |    1    |    5    |
|    6    |    4    |   15    |
|   10    |    9    |         |

x=10，y=9，z=15

3、先执行p2中的z=z+x，然后再执行p1中的y=z+y；

| x.value | y.value | z.value |
| :-----: | :-----: | :-----: |
|    1    |    1    |    5    |
|    6    |    4    |   15    |
|   10    |   19    |         |

x=10，y=19，z=15

##### 22、

![截屏2021-05-11 下午9.23.42](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午9.23.42.png)

1、使用信号量与P、V操作

```c
int Max=M;
semaphore wait,muteX;
int number_sum=0;
wait:=0,muteX:=1;
cobegin
 		process reader_i(int number){
  	P(mutex);
  	while( number_sum+number>=max ){
      V(muteX);
      P(wait);
    }
  	number_sum+=number;
  	V(muteX);
  	ReadFile/.../
    P(mutex);
  	number_sum-=number;
  	V(wait);
  	V(muteX);
}
coend
```



2、使用管程

```c
type sharefile = monitor{
  int numbersum=0,n;
  int max=M;
  condition SF;
  define startread,endread
  use wait,signal,check,release;
  InterfaceModule IM;
}
procedure startread(int number){
  begin
    check(IM);
  	while(numbersum+number>=max){
      wait(SF,IM);
    }
  	numbersum+=number;
  	release(IM);
  end
}

procedure endread(int number){
  begin
    check(IM);
  	numbersum-=number;
  	signal(SF,IM);
  	release(IM);
  end
}
cobegin
process read_i(){
  int number;
  number:=进程读文件编号
  startread(number);
  读文件
  endread(number);
}
coend
```
#####24
![截屏2021-05-11 下午10.21.32](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午10.21.32.png)

(1)

| 进程 | currentAvil    | Cki-Aki        | Allocation     | currentAvi+allocation | pos posible |
| ---- | -------------- | :------------- | -------------- | --------------------- | ----------- |
|      | A   B   C   D  | A   B   C   D  | A   B   C   D  | A   B   C   D         |             |
| P0   | 1   6    2   2 | 0   0    1   2 | 0   0    3   2 | 1   6    5   4        | T           |
| P1   | 1   9    8   6 | 1   7    5   2 | 1   0    0   0 | 2   9    8   6        | T           |
| P2   | 2   9    8   6 | 2   3    5   6 | 1   3    5   4 | 3   12  13 10         | T           |
| P3   | 1   6    5   4 | 0   6    5   2 | 0   3    3   2 | 1   9    8   6        | T           |
| P4   | 3   12  13 10  | 0   6    5   6 | 0   0    1   4 | 3   12  14 14         | T           |

​	由上表分析可得，系统此时属于安全状态

（2）
| 进程 | currentAvil   | Cki-Aki        | Allocation     | currentAvi+allocation | pos posible |
| ---- | ------------- | :------------- | -------------- | --------------------- | ----------- |
|      | A   B   C   D | A   B   C   D  | A   B   C   D  | A   B   C   D         |             |
| P0   |               | 0   0    1   2 | 0   0    3   2 |                       |             |
| P1   |               | 1   7    5   2 | 1   0    0   0 |                       |             |
| P2   |               | 2   3    5   6 | 1   3    5   4 |                       |             |
| P3   |               | 0   6    5   2 | 0   3    3   2 |                       |             |
| P4   |               | 0   6    5   6 | 0   0    1   4 |                       |             |

​	如果P1先申请1 2 2 2，那剩余可用的资源会变为（0，4，0，0），上表会变化为下表：

| 进程 | currentAvil   | Cki-Aki        | Allocation     | currentAvi+allocation | pos posible |
| ---- | ------------- | :------------- | -------------- | --------------------- | ----------- |
|      | A   B   C   D | A   B   C   D  | A   B   C   D  | A   B   C   D         |             |
| P0   |               | 0   0    1   2 | 0   0    3   2 |                       |             |
| P1   |               | 0   5    3   0 | 2   2    2   2 |                       |             |
| P2   |               | 2   3    5   6 | 1   3    5   4 |                       |             |
| P3   |               | 0   6    5   2 | 0   3    3   2 |                       |             |
| P4   |               | 0   6    5   6 | 0   0    1   4 |                       |             |

然后发现满足不了任何一个进程的需求，所以系统并不会分配资源给它



##### 29

![截屏2021-05-11 下午10.36.36](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-11 下午10.36.36.png)

```c
BEGIN
    integer mutex,avail[n2],full[n2];
    integer  i;
    mutex : =1;
   	for  i :=1 to n2  do
   	begin
     	avail [i] := m;
     	full [i] := 0;
    end;

    procedure  send(M)
		  integer i;
      begin
       		for i :=1 to n2  do
        	begin   
           	P(avail [i]);
        	end ;
       		P (mutex);
        	将消息放入缓冲区;
 					for  i :=1 to n2  do
					begin
						V(full [i]);
    			end ;
					V (mutex)

			end ;

 		procedure  receive(M,i)
    begin
				P (full[i]);
				P (mutex);
 				从缓冲区中取消息;
       	V (avail [i]);
				V (mutex);

		end ;

	Cobegin
   	Ai:
		begin
   	……
     	send (M)
   	………
  	end

  	Bi:
		begin
		……
			Receive(M,i);
		……
		end;
	Coend;
end;

```


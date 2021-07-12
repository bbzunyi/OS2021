# OS第五章课后作业

|  姓名  |   学号    |        院系        |       邮箱        |
| :----: | :-------: | :----------------: | :---------------: |
| 张洋彬 | 191220169 | 计算机科学与技术系 | 1016466918@qq.com |

## 一、应用题
10、
##### ![截屏2021-06-17 下午1.49.42](/Users/mac/Desktop/截屏2021-06-17 下午1.49.42.png)

1）实现一次页面访问的存取需要对内存进行两次存取，第一次访问页表，第二次访问对应的物理地址，所以需花费2.4$\mu$*s*

2）访问相联存储器时，只需要经过一次访问，在相联存储器的查找时间忽略不计，所以平均时间=1.2\*0.8+2.4\*0.2=1.44$\mu$*s*



17、
![IMG_3443](/Users/mac/Downloads/IMG_3443.jpg)

​		若页长为8kb，页表可有2^48^/2^13^=2^35^个页表项，反置页表有2^32^/2^13^=2^19^个页表项





21、

![截屏2021-06-17 下午2.16.32](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午2.16.32.png)

FIFO：

​	是先进先出的，所以会淘汰掉page3，由于D为1，要将该页写回磁盘

LRU：

​	若按老化算法，page1的age最大，所以淘汰掉page1，未有改变，不用写回磁盘

NRU：

​	从R=0的page中进行选择，和LRU一样，淘汰掉page1



30、

![截屏2021-06-17 下午2.44.16](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午2.44.16.png)

1）假设以0开始的100个B为P0，以此类推P1、P2……，访问序列为P0,P0,P1,P1,P0,P3,P1,P2,P2,P4,P4,P3

2)采用FIFO：页表的变化如下

![截屏2021-06-17 下午2.57.13](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午2.57.13.png)

缺页中断率为：5/12=41.67%

采用LRU：

![截屏2021-06-17 下午2.59.30](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午2.59.30.png)

缺页中断率为：6/12=50%





39、

![截屏2021-06-17 下午3.00.31](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午3.00.31.png)

1）

1052: 0000 0100 0001 1100 页号为1，页框号为7，所以物理地址为 0001 1100 0001 1100

2）

2221: 0000 1000 1010 1101 页号为2，此时发生缺页异常

3）

5499: 0001 0101 0111 1011 页号为5，页框号为0，所以物理地址为0000 0001 0111 1011



47、

![截屏2021-06-17 下午3.12.43](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午3.12.43.png)

1）OPT：

![截屏2021-06-17 下午3.40.48](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午3.40.48.png)

缺页次数：6

2）FIFO：

![截屏2021-06-17 下午3.42.59](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午3.42.59.png)

缺页次数：10

3）SCR：

![截屏2021-06-17 下午3.50.15](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午3.50.15.png)

缺页次数：10

4）改进的clock：

![截屏2021-06-17 下午4.05.36](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午4.05.36.png)

\*代表指针的位置，缺页次数：9

5）LRU：

![截屏2021-06-17 下午4.08.21](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午4.08.21.png)

缺页次数：7

6）MIN（滑动窗口$\gamma$=3）:

![截屏2021-06-17 下午4.16.06](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午4.16.06.png)

缺页次数：9

7）WS（工作集窗口尺寸$\Delta$=2）工作集初始为空：

![截屏2021-06-17 下午4.27.24](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-17 下午4.27.24.png)

缺页次数：9


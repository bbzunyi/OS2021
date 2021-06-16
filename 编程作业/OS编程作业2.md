# OS编程作业2

| 姓名   | 学号      | 邮箱              | 院系               |
| ------ | --------- | ----------------- | ------------------ |
| 张洋彬 | 191220169 | 1016466918@qq.com | 计算机科学与技术系 |

## 一、算法并行化问题

1、不考虑线程的话，算法的核心代码如下

```c
void merge(int left, int right){
    int mid = (left + right) >> 1;
    int size1 = mid - left + 1;
    int size2 = right - mid;
    int t1[size1];
    int t2[size2];

    memcpy(t1, a+left, sizeof(int) * (mid-left+1));
    memcpy(t2, a+mid+1, sizeof(int) * (right-mid));

    int i = 0, j = 0;
    int k = left;
    while (i < size1 && j < size2) {
        if (t1[i] <= t2[j]) {
            a[k] = t1[i];
            i++;
        }
        else {
            a[k] = t2[j];
            j++;
        }
        k++;
    }

    
    while (i < size1) {
        a[k] = t1[i];
        k++;
        i++;
    }

    while (j < size2) {
        a[k] = t2[j];
        j++;
        k++;
    }
}


void* merge_sort(void* arg){
    int *argu = (int*)arg;
    int left = argu[0];
    int right = argu[1];

    int mid = (left + right) >> 1;
    int arg1[2];
    int arg2[2];

        
    arg1[0] = left;
    arg1[1] = mid;

    arg2[0] = mid + 1;
    arg2[1] = right;

    if (left >= right) {
        return NULL;
    }
		merge_sort(arg1);
    merge_sort(arg2);
   
    merge(left, right);
    return NULL;
}


```

​	上述算法经测试，结果正确。

2、加入线程机制后，merge_sort的实现如下：

```c
 pthread_t t2;
    pthread_t t1;

    if (numofThread == maxThreadNumber) {
        flag = 1;
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1;
        pthread_create(&t1, NULL, merge_sort, (void*)arg1);
        pthread_join(t1, NULL);
     		pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg1);
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1;
        pthread_create(&t2, NULL, merge_sort, arg2);
        pthread_join(t2, NULL);
        pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg2);
    }
    
    merge(left, right);
```

将数据设置为100个，线程的数量设置0，1，10，20，100，结果如下图所示![截屏2021-05-13 下午11.10.21](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-13 下午11.10.21.png)

​	1000个数据，执行的次数为二叉树非叶子结点的个数，同时运行的线程个数应小于20，所以线程数量超过20时，将达不到最大线程数，线程的数量会导致开销增大，所以单线程的时间消耗较少，多线程消耗较多。

(Ps:由于每次运行结果都不一样，所以直接随机取值)



## 二、信号量与PV操作实现同步问题

​	实现的方式是写者优先，测试用例如下

![截屏2021-05-14 上午12.18.16](/Users/mac/Desktop/截屏2021-05-14 上午12.18.16.png)

![截屏2021-05-14 上午12.18.09](/Users/mac/Desktop/截屏2021-05-14 上午12.18.09.png)

​	由上图分析可得，虽然thread3和4先于5进入等待队列中，但是实现的方式是写者优先，5会在thread2写完后再去写，写操作是可以同步的，所以3、4一起运行读过程。



## 三、管程机制实现与管程应用问题

​		使用C++语言和pthread库实现一个有界环形缓冲区类(CircleBuffer), 该类提供一个带参(int K)构造方法，用于指定缓冲区的大小, 提供一个get和一个put方法分别用于从缓冲区取出一个元素和向缓冲区存入一个元素。另外需要编写一个多线程测试用例，测试该有界环形缓冲区类是否能够正确同步。具体实现要求：

1. 只能使用pthread库提供的一般信号量(semaphore)，参考课堂上介绍的使用信号量与PV操作实现Hoare类型管程，来实现CircleBuffer;

2. 使用pthread库提供的互斥信号量(mutex)和条件变量(condition)，实现CircleBuffer。

   没有去测试，代码如下：

   ```c
   #include <iostream>
   #include <ctype.h>
   #include <unistd.h>
   #include <stdio.h>
   #include <queue>
   #include <pthread.h>
   #include <stdlib.h>
   #include <string.h>
   #include <sys/time.h>
   #include <time.h>
   #include <boost.h>
   class mesa_monitor : boost::noncopyable {
   public:
       typedef boost::unique_lock<mesa_monitor> lock_type;
       friend class lock_type;
       mesa_monitor() : m_notify(0) {}
   public:
       void lock() const {
           m_mutex.lock();
           m_notify = 0;  // 进入管程时要把m_notify归0
       }
       void unlock() const {
           notify_impl(m_notify);
           m_mutex.unlock();
       }
       bool try_lock() const {
           bool ret = m_mutex.try_lock();
           if (ret) {
               m_notify = 0;
           }
           return ret;
       }
       void wait() const {
           notify_impl(m_notify);
           m_cond.wait(m_mutex);
           m_notify = 0;
       }
       void notify_one() {
           if (m_notify != -1) {
               ++m_notify;
           }
       }
       
       void notify_all() {
           m_notify = -1;
       }
       
   private:
       void notify_impl(int nnotify) const {
           if (nnotify != 0) {
               if (nnotify = -1) {
                   m_cond.notify_all();
                   return;
               } else {
                   while (nnotify > 0) {
                       m_cond.notify_one();
                       --nnotify;
                   }
               }
           }
       }
   private:
       mutable boost::condition_variable_any m_cond;
       mutable boost::mutex m_mutex;
       mutable int m_notify;
   };
   
   template <typename T>
   class threadsafe_queue : mesa_monitor {
       std::queue<T> m_data;
   public:
       threadsafe_queue() {}
       void pop(T& val) {
           mesa_monitor::lock_type lk(*this);
           while (m_data.empty()) {
               wait();
           }
           val = m_data.front();
           m_data.pop();
       }
       void push(const T& val) {
           mesa_monitor::lock_type lk(*this);
           m_data.push(val);
           notify_one();
       }
   };
   
   ```

   

## 四、死锁问题

​	两个进程分别是打印奇数和偶数，当A线程进入锁定状态是，主线程突然异常将A线程停止，这时将导致B线程也无法继续执行，处于死锁状态，如下图所示：

![截屏2021-05-14 上午12.39.56](/Users/mac/Library/Application Support/typora-user-images/截屏2021-05-14 上午12.39.56.png)

贴一段进程创建的代码实现

```c
 pthread_t todd,teven;
        pthread_mutex_init(&m,0);
        pthread_create(&todd,0,runodd,0);
        pthread_create(&teven,0,runeven,0);
        sleep(5);
        printf("stop todd from the outside\n");
        pthread_cancel(todd);
        pthread_join(todd,(void**)0);
        pthread_join(teven,(void**)0);
        pthread_mutex_destroy(&m);
```




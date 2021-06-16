# OS第四章编程作业

| 姓名   | 学号      | 邮箱              | 院系               |
| ------ | --------- | ----------------- | ------------------ |
| 张洋彬 | 191220169 | 1016466918@qq.com | 计算机科学与技术系 |

## 一、硬链接

​	实现要求：编写一个C语言程序scanner，扫描类UNIX文件系统，找到并定位所有硬链接数为2或以上的i节点。对于每个这样的文件，将所有指向该文件的文件名列在一起。



​	首先，创建类UNIX系统所需要的结构体

```c
struct filesys {
        unsigned int s_size;        //总大小
        unsigned int s_itsize;        //inode表大小
        unsigned int s_freeinodesize;    //空闲i节点的数量
        unsigned int s_nextfreeinode;    //下一个空闲i节点
        unsigned int s_freeinode[NUM];     //空闲i节点数组
        unsigned int s_freeblocksize;    //空闲块的数量
        unsigned int s_nextfreeblock;    //下一个空闲块
        unsigned int s_freeblock[NUM];    //空闲块数组
        unsigned int s_pad[];        //填充到512字节
};
//磁盘inode结构
struct finode {
        int fi_mode;            //类型：文件/目录
        int fi_nlink;            //链接数，当链接数为0，意味着被删除
        long int fi_size;        //文件大小
        long int fi_addr[BNUM];        //文件块一级指针，并未实现多级指针
};
//内存inode结构
struct inode {
        struct finode   i_finode;
        struct inode    *i_parent;    //所属的目录i节点
        int     i_ino;            //i节点号
        int     i_users;        //引用计数
};
//目录项结构(非Linux内核的目录项)
struct direct
{
        char d_name[MAXLEN];        //文件或者目录的名字
        unsigned short d_ino;        //文件或者目录的i节点号
};
//目录结构
struct dir
{
        struct direct direct[DIRNUM];    //包含的目录项数组
        unsigned short size;        //包含的目录项大小
};
//抽象的文件结构
struct file {
        struct inode *f_inode;        //文件的i节点
        int f_curpos;            //文件的当前读写指针
};
```



## 二、文件系统布局

- 实现一个C语言程序fat12，能够创建并格式化一个二进制文件(大小为1.44MB)为FAT12磁盘镜像，并能将指定的宿主文件系统中的文件拷贝到该磁盘镜像中指定的目录中（若目录不存在，则需要自动创建），也能从该磁盘镜像中拷贝出某个指定的文件。程序运行样例如下：

  - fat12 -f img.bin // 创建并格式化磁盘镜像文件img.bin
  - fat12 -mi img.bin /demo/ fileA.txt // 将宿主文件系统当前目录下文件fileA.txt拷贝到磁盘镜像img.bin的/demo目录中
  - fat12 -mo img.bin /demo/fileB.txt . // 将磁盘镜像img.bin中/demo/fileB.txt文件拷贝到宿主文件系统当前目录

  这一部分没时间和精力去完成了，愿谅解。
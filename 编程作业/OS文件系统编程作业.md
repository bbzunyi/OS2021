# OS文件系统编程作业

| 姓名   | 学号      | 邮箱              | 院系               |
| ------ | --------- | ----------------- | ------------------ |
| 张洋彬 | 191220169 | 1016466918@qq.com | 计算机科学与技术系 |

## 一、硬链接

​	编写一个C语言程序scanner，扫描类UNIX文件系统，找到并定位所有硬链接数为2或以上的i节点。对于每个这样的文件，将所有指向该文件的文件名列在一起。



首先需了解stat结构体![截屏2021-06-11 上午12.25.11](/Users/mac/Library/Application Support/typora-user-images/截屏2021-06-11 上午12.25.11.png)

​		可见包含了上述内容，我们要做的就是扫描st_nlink。

​	递归扫描文件夹，代码如下：

```c
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
void dir_oper_print(int ino,char*path){
	struct  dirent*filename;
    struct stat s_buf;
    DIR *dp;
	if(!(dp= opendir(path)))
		return;
	if(stat(path,&s_buf)<0 || !S_ISDIR(s_buf.st_mode))
		return;
    while(filename = readdir(dp))
	{
		/*判断一个文件是目录还是一个普通文件*/
		char file_path[4096];
		bzero(file_path,4096);
		strcat(file_path,path);
		strcat(file_path,"/");
		strcat(file_path,filename->d_name);
		
		/*在linux下每一个目录都有隐藏的. 和..目录，一定要把这两个排除掉。因为没有意义且会导致死循环*/
		if(strcmp(filename->d_name,".")==0||strcmp(filename->d_name,"..")==0)
		{
			continue;
		}
 
		/*获取文件信息，把信息放到s_buf中*/
		stat(file_path,&s_buf);
		if(s_buf.st_ino==ino){
			printf("%s\n ",file_path);
		}
		/*判断是否目录*/ 
		if(S_ISDIR(s_buf.st_mode))
		{
			dir_oper_print(ino,file_path);//递归调用
		}
 
	
    }
	closedir(dp);
	return;
}
void dir_oper(char const*path){
    struct  dirent*filename;
    struct stat s_buf;
    DIR *dp;
	if(!(dp= opendir(path)))
		return;
	if(stat(path,&s_buf)<0 || !S_ISDIR(s_buf.st_mode))
		return;
	
    while(filename = readdir(dp))
	{
		/*判断一个文件是目录还是一个普通文件*/
		char file_path[4096];
		bzero(file_path,4096);
		strcat(file_path,path);
		strcat(file_path,"/");
		strcat(file_path,filename->d_name);
		
		/*在linux下每一个目录都有隐藏的. 和..目录，一定要把这两个排除掉。因为没有意义且会导致死循环*/
		if(strcmp(filename->d_name,".")==0||strcmp(filename->d_name,"..")==0)
		{
			continue;
		}
 
		/*获取文件信息，把信息放到s_buf中*/
		stat(file_path,&s_buf);
		if(s_buf.st_nlink>=2){
			printf("inode:%ld",s_buf.st_ino);
			dir_oper_print(s_buf.st_ino,"/home");
			printf("\n");
		}
		/*判断是否目录*/ 
		if(S_ISDIR(s_buf.st_mode))
		{
			dir_oper(file_path);//递归调用
		}

    }
	closedir(dp);
	return;
}
int main(int argc, char const *argv[]){
    char const*path = "/home";
	struct stat s_buf;
 
	/*获取文件信息，把信息放到s_buf中*/
	stat(path,&s_buf);
 
	
	if(S_ISDIR(s_buf.st_mode))
	{
		dir_oper(path);
	}
 
	else if(S_ISREG(s_buf.st_mode))
	{
		if(s_buf.st_nlink>=2){
            printf("%s\n",path);
        }
		return 0;
	}
 
	return 0;

}
```

​	扫描现有系统，结果如下：![截屏2021-06-13 下午1.12.07](/Users/mac/Desktop/截屏2021-06-13 下午1.12.07.png)

​	可见代码执行正确。



## 二、文件系统布局

​	尝试了一下发现写不出来，老师请见谅。
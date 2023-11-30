# OS_shell

## 目前进展：
> ls 命令 支持 -l -a -al/la参数
> 
> cd 命令 
> 
> cp 命令 支持复制文件 -r复制文件夹
> 
> mv 命令 支持移动、重命名文件，文件夹
>
> rm 命令 支持 删除文件 -r删除文件夹
>
> pwd 命令 
>
> history命令
>
> 输入输出重定向
>
> tree 命令 支持 -a 参数
>
> pipe 
>
> cat 命令 支持读取文件内容
>
> clear 清屏
>
> mkdir 命令 创建目录 -p递归创建 -m设置权限
>
> ps 命令 输出进程信息
>
> help 命令
>
> man 命令
>
> toupper、tolower 将字符串全部转化为大写或小写，主要用于测试管道、重定向等命令
>
> & 后台运行程序，直接使用system函数，因此同时也实现了外部命令调用
>
> head命令，显示文件的前n行内容，如果不传行数默认显示前4行
>
> tail命令，显示文件后n行内容，如果不传行数默认显示前4行
>
> touch命令，新建文件，若已存在只更新时间戳内容不变
>
> vim命令，进入vim编辑其编辑文件

## 一些提醒

### How to use
使用命令
```
gcc main.c -I ./src src/*/*.c
```
或者
```
clang main.c -I ./src src/*/*.c
```
将会得到`a.out`文件
运行文件
```
./a.out
```

### 增加一个shell命令的步骤
1. 在`shell.h`中新增一个调用号
```
#define SYS_HISTORY 99
```
但是注意不要与已有的冲突，除此外是可以随便取的

</br>

2. 在`shell.h`中的`sys_table[]`注册函数
```
[SYS_HISTORY] = (syscall_handler_t)syscall_history,
```
左边是刚刚定义的宏，右边是待实现的函数

</br>

3. 在`shell.h`中添加函数声明
```
void syscall_history();
```

</br>

4. 在`shell.h`中的`cmd_list[] `注册命令
```
{
    .name = "history",
    .id = SYS_HISTORY,
},
```
其中name是命令的名字，id是刚刚定义的宏

</br>

5. 前往文件夹`./src`中新建一个文件夹,其中包含函数定义的`.c`文件，以及相关声明的`.h`头文件

</br>

6. 在`shell.h`中包含此命令的`.h`头文件

</br>

7. 如果需要定义一些全局变量，在`.h`头文件中用extern声明，在`.c`中定义

</br>


### 图标
```
   /Users/yongrin/Desktop/SYSU/操作系统/my_shell/OS_shell
>>>
```
![](imgs/1.png)

但只做了苹果的图标，因此理论上只有苹果用户可以看见，Linux用户请前往`main.c`中约13行位置，即`while`循环里第一个`printf`处，将苹果图标进行替换。同时需要下载`hack nerd font`字体

## 调试进展： 
> 已完成：history ls cd pwd mkdir tree clear cat ps
> 
> cd新增：cd ～ 切换到home目录
> 
> mkdir新增：创建多个目录、-p递归创建、-m设置权限、忽略已存在目录
>
> 修改cp,支持：
> cp 1.txt 2.txt / cp 1.txt dir/2.txt ： 复制并重命名文件
> cp 1.txt dir ： 复制文件到制定目录下
> cp -r dir1 dir2 ： 复制目录1到目录2（dir2不存在就新建，存在就复制到dir2下）
> 分3个辅助函数：copy_file_to_dir、copy_file_to_file、copy_folder
>
> mv 支持文件、目录

## 调试TUDO：



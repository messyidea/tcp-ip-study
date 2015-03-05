linux IO
---

##标准IO对比于系统函数

#优点
*标准IO更易于移植
*标准IO可以利用缓冲区提高性能

#缺点
*不易进行双向通信
*切换模式的时候会频繁调用fflush函数降低性能
*需要用FILE结构体指针的形式返回文件描述符

```c
FILE * fdopen(int fildes, const char  * mod);
// fildes 需要转换的文件描述符
// mod 模式信息

int fileno(FILE * stream)
//成功时返回文件描述符，失败时返回-1
```

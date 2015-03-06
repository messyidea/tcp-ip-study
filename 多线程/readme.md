多线程
---


#编译方法
```c
gcc thread1.c -o tr1 -lpthread


gcc semaphore.c -D_REENTRANT -o sema -lpthread
```


#销毁线程方法
```c
int pthread_detach(pthread_t thread);
```

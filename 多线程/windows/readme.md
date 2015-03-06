windows 多线程
---

* 操作系统创建的资源(比如说进程，线程，文件，信号量，互斥量等)都是由windows操作系统创建并管理的资源。windows以记录相关信息的方式管理各种资源。再起内部生成数据块(每种资源所拥有的数据块结构有所差异)，这种数据块称为内核对象。

* 内核对象的创建、管理以及销毁时机的决定等工作都是由操作系统完成的。

#windows 创建线程的方法
```c
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,   //线程安全相关信息，默认为NULL
    SIZE_T dwStackSize,                         //要分配线程的栈的大小，传递0时生成默认大小的栈
    LPTHREAD_START_ROUTINE lpStartAddress,      //传递线程的main函数信息
    LPVOID lpParameter,                         //调用main函数时传递的参数信息
    DWORD dwCreationFlags,                      //用于制定线程创建后的行为，0时，线程创建后立即进入可执行状态
    LPDWORD lpThreadId                          //用于保存线程ID的变量地址值
)
```

#如果线程要调用c/c++标准库函数，则需要用如下函数创建线程
```c
uintptr_t _beginthreadex(...);
```

#内核状态查看
```c
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
//hHandle 查看状态的内核对象句柄
//dwMilliseconds 指定超时

DWORD WaitForMultipleObjects(...);  //验证多个内核对象状态
```


#同步
```c
void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);


HANDLE CreateMutex(...);    //创建互斥量
BOOL CloseHandle(HANDLE hObject);   //销毁内核对象

BOOL ReleaseMutex(HANDLE hMutex);   //释放信号量，活取可以用WaitForSingleObject完成

HANDLE CreateSemaphote(...);
BOOL ReleaseSemaphore(...);


//基于事件对象的同步
HANDLE CreateEvent(...);
BOOL ResetEvent(HANDLE hEvent);     //to the non-signaled
BOOL SetEvent(HANDLE hEvent);       //to the signaled
```

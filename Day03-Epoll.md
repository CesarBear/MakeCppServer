# I/O复用  
> Linux下实现I/O复用的系统调用主要有select、poll和epoll  

## 场景  
- 客户端要同时处理多个socket。比如非阻塞connect技术  
- 客户端要同属处理用户输入和网络连接。比如聊天室程序  
- TCP服务器要同时处理监听socket和连接socket。**这是I/O复用使用最多的场景**  
- 服务器要同时处理TCP请求和UDP请求。比如回射服务器  
- 服务器要同时监听多个端口，或者处理多种服务。比如xinetd服务器  

## select系统调用  
> 用途：在一段指定时间内，监听用户感兴趣的文件描述符上的可读、可写和异常等事件  

### select API  
```
#include <sys/select.h>  
int select( int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout );  
```
1. ```nfds``` 指定被监听的文件描述符的总数。通常被设置为select监听的所有文件描述符中的最大值加1，因为文件描述符是从0开始计数的  
2. ```readfds```、```writefds```和```exceptfds```分别指向可读、可写和异常等事件对应的文件描述符集合  
3. ```timeout```用来设置select函数的超时时间  
> select成功时返回就绪文件描述符的总数。失败时返回-1并设置errno。如果在select等待期间，程序接收到信号，则select立即返回-1，并设置errno为EINTR  

### 文件描述符就绪条件  
- socket可读：  
1) socket内核接收缓存区中的字节数大于或等于其低水位标记SO_RCVLOWAT  
2) socket通信的对方关闭连接  
3) 监听socket上有新的连接请求  
4) socket上有未处理的错误  
- socket可写  
1) socket内核发送缓存区中的可用字节数大于或等于其低水位标记SO_SNDLOWAT  
2) socket的写操作被关闭。对写操作被关闭的socket执行写操作将触发一个SIGPIPE信号  
3) socket使用非阻塞connect连接成功或者失败（超时）之后  
4) socket上有未处理的错误  
- select唯一能处理的异常：socket上接收到带外数据  

### 处理带外数据  
- 对于可读事件，采用普通的```recv```函数读取数据  
```
ret = recv( connfd, buf, sizeof( buf ) -1, 0 );
```
- 对于异常事件，采用带**MSG_OOB**标志的```recv```函数读取带外数据  
``` 
ret = recv( connfd, buf, sizeof( buf )-1, MSG_OOB );
``` 

## poll系统调用  
> 和select类似，也是在指定时间内轮询一定数量的文件描述符，以测试其中是否有就绪者  

### poll API 
```
#include <poll.h> 
int poll( struct pollfd* fds, nfds_t nfds, int timeout );
``` 
1. ```fds```是一个pollfd结构类型的数组，指定所有我们感兴趣的文件描述符上发生的可读、可写和异常事件  
2. ```nfds```指定被监听时间集合fds的大小  
3. ```timeout```指定poll的超时值，单位是毫秒  

## epoll系列系统调用  
> epoll时Linux特有的I/O复用函数，在实现和使用上与select、poll有很大差异。  
> 首先，epoll使用一组函数来完成任务，而不是单个函数  
> 其次，epoll把用户关心的文件描述符上的事件放在内核里的一个事件表中，从而无需像select和poll那样重复轮询  

### 内核事件表  
> epoll需要使用一个额外的文件描述符，来唯一标识内核中的这个时间表  
``` 
#include <sys/epoll.h>
int epoll_create( int size )
``` 
```size```参数现在并不起作用，只是给内核一个提示，告诉它事件表需要多大。  
该函数返回的文件描述符将用作其他所有epoll系统调用的第一个参数，以指定要访问的内核事件表  

### 操作epoll内核事件表的函数  
```
#include <sys/epoll.h> 
int epoll_ctl( int epfd, int op, int fd, struct epoll_event *event ) 
``` 
```op```指定操作类型，有以下3类：
- EPOLL_CTL_ADD，往事件表中注册fd上的事件  
- EPOLL_CTL_MOD，修改fd上的注册事件  
- EPOLL_CTL_DEL，删除fd上的注册事件  

```event```指定事件。epoll支持的事件类型和poll基本相同；但epoll有两个额外的事件类型——**EPOLLET**和**EPOLLONESHOT**  

### epoll_wait函数  
> 主要接口。它在一段超时时间内等待一组文件描述符上的事件  
```
#include <sys/epoll.h>
int epoll_wait( int epfd, struct epoll_event* events, int maxevents, int timeout );
``` 
当检测到事件，就将所有就绪的事件从内核事件表中复制到第二个参数```events```指向的数组中。
> 这个数组只用于输出epoll_wait检测到的就绪事件，而不像select和poll的数组参数那样既用于传入用户注册的事件，用用于输出内核检测到的就绪事件  

### LT和ET模式  
> epoll对文件描述符的操作有两种模式：LT（Level Trigger）模式和ET（Edge Trigger）模式  
> LT模式是默认工作模式，相当于 一个效率较高的poll；ET模式是高效工作模式  

- LT——应用程序可以不立即处理事件，epoll_wait每一次都会向应用程序通告此事件，直到该事件被处理  
- ET——epoll_wait检测到其上有事件发生并将此事件通知应用程序后，应用程序必须立即处理该事件  
> ET模式很大程度上降低了同一个epoll事件被重复触发的次数，因此效率较高  

### EPOLLONESHOT事件  
> 尽管一个socket在不同时间可能被不同的线程处理，但同一时刻肯定只有一个线程为它服务  

对于注册了EPOLLONESHOT事件的文件描述符，操作系统最多触发其上注册的一个可读、可写或者异常事件，且只触发一次，
除非我们使用epoll_ctl函数重置该文件描述符上注册的EPOLLONESHOT事件。  
因此，注册了EPOLLONESHOT事件的socket一旦被某个线程处理完毕，该线程就应该立即重置这个socket上的EPOLLONESHOT事件，
以确保这个socket下次可读时，其EPOLLIN事件能被触发，进而让其他工作线程有机会继续处理这个socket  










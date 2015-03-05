#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *message);
void setnonblockingmode(int fd);

int main(int argc,char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if(argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    
    setnonblockingmode(serv_sock);  //important!
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    //epoll例程epfd注册文件描述符serv_sock, 主要监视event事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1) {
        //epoll_wait(epoll例程, 保存发生的事件的结构体地址, 第二个参数中保存的最大的事件数, timeout);
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }
        
        puts("return epoll_wait");
        for(i = 0; i<event_cnt; i++) {
            if(ep_events[i].data.fd == serv_sock) {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                setnonblockingmode(clnt_sock);  //important
                event.events = EPOLLIN|EPOLLET; //change
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            } else {    //边缘触发需要读取缓冲区所有数据，所以要循环
                while(1){
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                        break;
                    } else if(str_len < 0) {
                        if(errno == EAGAIN) break;  //表示读取了缓冲区所有的数据
                    } else {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }

    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char * message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void setnonblockingmode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char * argv[]) {
    int fds[2];
    char str1[] = "who are you?";
    char str2[] = "thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid == 0) {
        write(fds[1], str1, sizeof(str1));
        sleep(2);            //不能注释，先读的进程会把数据读走，子进程自己读走了数据，父进程循环等待。
        read(fds[0], buf, BUF_SIZE);
        printf("child proc output: %s \n",buf);
    } else {
        read(fds[0], buf, BUF_SIZE);
        printf("father proc output: %s \n",buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);           //为了让父进程不先结束
    }
}

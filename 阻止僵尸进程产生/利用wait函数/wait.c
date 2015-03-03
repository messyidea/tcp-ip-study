#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();

    if(pid == 0) {
        return 3;
    } else {
        printf("children pid: %d\n",pid);
        pid = fork();
        if(pid == 0) {
            exit(7);
        } else {
            printf("children pid: %d\n",pid);
            wait(&status);
            if(WIFEXITED(status)) {
                printf("child send one: %d\n",WEXITSTATUS(status));
            }

            wait(&status);
            if(WIFEXITED(status)) 
                printf("child send two: %d\n",WEXITSTATUS(status));

            sleep(30);
        }
    }
}

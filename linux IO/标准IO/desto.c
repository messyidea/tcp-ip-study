#include <stdio.h>
#include <fcntl.h>

int main() {
    FILE *fp;
    int fg = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if(fg == -1) {
        fputs("file open error", stdout);
        return -1;
    }

    fp = fdopen(fg, "w");
    fputs("Network C programming \n", fp);
    fclose(fp);
    return 0;
}

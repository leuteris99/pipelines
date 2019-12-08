#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    pid_t pid;
    int bufSize = 128;
    char buf[bufSize];
    int p[2];
    pipe(p);

    if ((pid = fork()) > 0) {
        char *data = "this is a text";
        /*int fd = open(argv[1], O_RDONLY); // TODO: add semaphore. Reading the file is a critical section.
        if (read(fd, data, bufSize)) {
            printf("Readied successful: %s\n", data);
        }*/
        // parent
        printf("hi from parent\n");
        write(p[1], data, strlen(data) + 1);
        //close(fd);
    } else if (pid == 0) {
        //child
        printf("hi from child\n");
        read(p[0], buf, bufSize);
        printf("the data is: %s\n", buf);
    }


    return 0;
}

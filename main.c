#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    pid_t pid;
    int bufSize = 4096;
    char buf[bufSize];
    int p[2],pa[2];

    if (pipe(p) < 0) {
        perror("Pipe error: ");
        return 1;
    }
    if (pipe(pa) < 0) {
        perror("Pipe error: ");
        return 1;
    }

    if ((pid = fork()) > 0) {
        // parent
        printf("hi from parent\n");
        printf("enter critical section parent!\n");
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0){
            perror("open file error: ");
            return 1;
        }
        int a = read(fd, buf, bufSize);
        printf("%d\n",a);
        if (a > 0) {
            printf("Readied successful: %s\n", buf);
        }

        close(p[0]); //pre
        dup2(p[1], STDOUT_FILENO); // post
        close(p[1]); //post
        //write(p[1], buf, strlen(buf) + 1); //pre
        close(fd);
        //printf("complete parent!\n");
        printf("%s",buf);
        char toPrint[bufSize];
        close(pa[1]);
        dup2(pa[0],STDIN_FILENO);
        close(pa[0]);
        read(STDIN_FILENO,toPrint,bufSize + 1);
        printf("i am your father");
        printf("%s",toPrint);
    } else if (pid == 0) {
        //child
        printf("hi from child\n");
        printf("enter critical section child!\n");

        close(p[1]); // pre
        dup2(p[0],STDIN_FILENO); // post
        close(p[0]); //post
        //read(p[0], buf, bufSize); //pre
        printf("the data is: %s\n", buf);
        printf("complete child!\n");
        //char *args[]={"tr","\"[:lower:]\"","\"[:upper:]\""}; // TODO: fix the exec.

        execl("/usr/bin/tr","tr","[:lower:]","[:upper:]",NULL);
        //execl("/bin/ls","ls","-l",NULL);
        printf("the exec didn't work :(");
    } else{
        perror("fork error: ");
        return 1;
    }


    return 0;
}

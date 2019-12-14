#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/**
@author Eleftherios Alexiou (P2017143)
@author Nemanja Jevtić (P2017182)

@since 14/12/2019

This code takes a file as an argument, it reads it and uses a pipe to run the tr
shell command on a new process, replacing the lower case letters with upper case.
Than it returns the output and print it to the parent process.
*/
int main(int argc, char **argv) {
    pid_t pid; // pid to determine the current process
    int bufSize = 4096; // buffer size
    char buf[bufSize]; // buffer to store the data from the file
    int p[2]; // pipe which sends data from the parent to the children.

    if (pipe(p) < 0) { // create the pipeline
        perror("Pipe error: ");
        return 1;
    }

    if ((pid = fork()) > 0) { // create the child process
        // parent
        int fd = open(argv[1], O_RDONLY); // opening the file
        if (fd < 0){
            perror("open file error: ");
            return 1;
        }
        int a = read(fd, buf, bufSize); // read from the file
        printf("The file contains:\n");
        /*if (a > 0) {
            printf("Readied successful: %s\n", buf); // Debug
        }*/

        close(p[0]);
        dup2(p[1], STDOUT_FILENO); // redirect pipe to standard output
        close(p[1]);
        close(fd);
        printf("%s",buf); // print the processed data
        char toPrint[bufSize];

        read(STDIN_FILENO,toPrint,bufSize); // return the output of from the child

        return 0;
    } else if (pid == 0) {
        //child
        close(p[1]);
        dup2(p[0],STDIN_FILENO); // redirect pipe from standard input
        close(p[0]);

        execl("/usr/bin/tr","tr","[:lower:]","[:upper:]",NULL); // execute the tr command
        printf("the exec didn't work :("); // error message if exec fails
    } else{
        perror("fork error: ");
        return 1;
    }

    return 0;
}

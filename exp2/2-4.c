#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define INPUT 0
#define OUTPUT 1

int main() {
    int file_descriptors[2];
    pid_t pid; 
    char *msg1 = "Child 1 is sending a message!\n"; 
    char *msg2 = "Child 2 is sending a message!\n";

    char buf[256]; 
    int read_count = 0 , result = 0;

    result = pipe(file_descriptors);
    if (result == -1) {
        perror("Failed in calling pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("Failed in calling fork");
        exit(1);
    } 
    else if (pid == 0) {
        close(file_descriptors[INPUT]);
        result = write(file_descriptors[OUTPUT], msg1, strlen(msg1));
        if (result == -1) {
            perror("In Child1, failed to write to pipe");
            exit(1);
        }
        close(file_descriptors[OUTPUT]);
        exit(0);
    } 
    else {
        pid = fork();
        if (pid == -1) {
            perror("Failed in calling fork");
            exit(1);
        } 
        else if (pid == 0) {
            close(file_descriptors[INPUT]);
            result = write(file_descriptors[OUTPUT], msg2, strlen(msg2));
            if (result == -1) {
                perror("In Child2, failed to write to pipe");
                exit(1);
            }
            close(file_descriptors[OUTPUT]);
            exit(0);
        } 
        else {
            close(file_descriptors[OUTPUT]);
            while ((read_count = read(file_descriptors[INPUT], buf, sizeof(buf) - 1)) > 0) {
                buf[read_count] = '\0';
                printf("%d bytes of data received from child process: %s\n", read_count, buf);
            }
            if (read_count == -1) {
                perror("In parent, failed to read from pipe");
                exit(1);
            }
            close(file_descriptors[INPUT]);
        }
    }

    return EXIT_SUCCESS;
}

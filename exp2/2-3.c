#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    pid_t pid1, pid2;

    printf("Original\n");
    pid1 = fork();

    if (pid1 < 0) {
        perror("Failed to fork first child");
        exit(1);
    } 
    else if (pid1 == 0) {
        printf("Child 1\n");
        return EXIT_SUCCESS;
    } 
    else {
        printf("Parent\n");

        pid2 = fork();

        if (pid2 < 0) {
            perror("Failed to fork second child");
            return EXIT_FAILURE;
        } 
        else if (pid2 == 0) {
            printf("Child 2\n");
            return EXIT_SUCCESS;
        }

        wait(NULL);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}

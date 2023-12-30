#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int result;
    pid_t pid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("Failed in calling fork");
        exit(1);
    } 
    else if (pid == 0) {
        result = execvp(argv[1], &argv[1]);

        if (result == -1) {
            perror("Failed in execvp");
            exit(1);
        }
        exit(0);
    } 
    else {
        int status;

        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit successfully\n");
        }
    }

    return EXIT_SUCCESS;
}

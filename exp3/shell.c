#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 100
#define MAX_ARGS 100

int main() {
    char input[MAX_LENGTH];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status = 0, i = 0;

    while (1) {
        printf("21071125YSM's Shell> ");

        if (fgets(input, MAX_LENGTH, stdin) == NULL) {
            break;
        }

        if (strcmp(input, "quit\n") == 0) {
            break;
        }

        token = strtok(input, " \t\n");

        i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }

        //args last elem must be NULL
        args[i] = NULL; 

        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0) {
            if (execvp(args[0], args) < 0) {
                perror("execvp failed");
                exit(1);
            }
        } 
        else {
            wait(NULL);
            printf("Child exited successfully\n");
        }
    }

    return 0;
}

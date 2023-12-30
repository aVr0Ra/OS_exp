#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sigint_handler(int sig) {
    printf("Caught signal %d in process %d\n", sig, getpid());
}

int main() {
    pid_t pid;

    // register SIGINT 
    signal(SIGINT, sigint_handler);

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("Child process (PID %d) waiting for SIGINT...\n", getpid());
        pause();
    } 
    else {
        printf("Parent process (PID %d) waiting for SIGINT...\n", getpid());
        pause();
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shmPtr;

    // create shared mem
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // 将共享内存映射到进程的地址空间
    shmPtr = (char *)shmat(shmid, NULL, 0);
    if (shmPtr == (char *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    memset(shmPtr, 0, SHM_SIZE);
    strncpy(shmPtr, "Hello, Shared Memory!", SHM_SIZE);

    if (shmdt(shmPtr) == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

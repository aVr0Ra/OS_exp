#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shmPtr;

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // 将共享内存映射到进程的地址空间
    shmPtr = (char *)shmat(shmid, NULL, 0);
    if (shmPtr == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // 读取共享内存中的数据并打印
    printf("Data read from shared memory: %s\n", shmPtr);

    // 解除映射
    if (shmdt(shmPtr) == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    // 删除共享内存段
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl(IPC_RMID) failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

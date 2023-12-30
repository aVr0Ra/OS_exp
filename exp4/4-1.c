#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myThread1(void *vargp) {
    printf("Thread 1 is running\n");
    return ;
}

void *myThread2(void *vargp) {
    printf("Thread 2 is running\n");
    return ;
}

int main(int argc , char **argv) {
    pthread_t thread_id1, thread_id2;

    if (pthread_create(&thread_id1, NULL, myThread1, NULL) != 0) {
        perror("Failed to create thread 1");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread_id2, NULL, myThread2, NULL) != 0) {
        perror("Failed to create thread 2");
        return EXIT_FAILURE;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    printf("Both threads finished executing\n");
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printInt(void *arg) {
    int value = *(int *)arg;
    printf("Thread with integer: %d\n", value);
    return NULL;
}

void *printChar(void *arg) {
    char value = *(char *)arg;
    printf("Thread with character: %c\n", value);
    return NULL;
}

int main() {
    pthread_t thread_id1, thread_id2;
    int intArg = 123;
    char charArg = 'A';

    if (pthread_create(&thread_id1, NULL, printInt, &intArg) != 0) {
        perror("Failed to create thread 1");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread_id2, NULL, printChar, &charArg) != 0) {
        perror("Failed to create thread 2");
        return EXIT_FAILURE;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    printf("Both threads finished executing\n");
    return EXIT_SUCCESS;
}

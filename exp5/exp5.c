#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define NUMBERS 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in, out;

    sem_t empty, full;

    pthread_mutex_t mutex;
}BufferPool;

BufferPool bufferPool;

void *producer(void *param) {
    int producerId = *((int *)param);
    FILE *file = fopen(producerId == 0 ? "data1.txt" : "data2.txt", "r");

    int number;
    for (int i = 0; i < NUMBERS; i++) {
        fscanf(file, "%d", &number);

        sem_wait(&bufferPool.empty);
        pthread_mutex_lock(&bufferPool.mutex);

        bufferPool.buffer[bufferPool.in] = number;
        bufferPool.in = (bufferPool.in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&bufferPool.mutex);
        sem_post(&bufferPool.full);
    }

    fclose(file);
    return NULL;
}

void *consumer_sum(void *param) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int num1, num2;

        sem_wait(&bufferPool.full);
        pthread_mutex_lock(&bufferPool.mutex);
        num1 = bufferPool.buffer[bufferPool.out];
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&bufferPool.mutex);
        sem_post(&bufferPool.empty);

        sem_wait(&bufferPool.full);
        pthread_mutex_lock(&bufferPool.mutex);
        num2 = bufferPool.buffer[bufferPool.out];
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&bufferPool.mutex);
        sem_post(&bufferPool.empty);

        printf("Sum: %d + %d = %d\n", num1, num2, num1 + num2);
    }
    return NULL;
}

void *consumer_product(void *param) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int num1, num2;

        sem_wait(&bufferPool.full);
        pthread_mutex_lock(&bufferPool.mutex);
        num1 = bufferPool.buffer[bufferPool.out];
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&bufferPool.mutex);
        
        sem_post(&bufferPool.empty);

        sem_wait(&bufferPool.full);
        pthread_mutex_lock(&bufferPool.mutex);
        num2 = bufferPool.buffer[bufferPool.out];
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&bufferPool.mutex);

        sem_post(&bufferPool.empty);

        printf("Product: %d * %d = %d\n", num1, num2, num1 * num2);
    }
    return NULL;
}

int main() {
    pthread_t tidA, tidB, tidC, tidD;
    int producerId1 = 0, producerId2 = 1;

    sem_init(&bufferPool.empty, 0, BUFFER_SIZE);
    sem_init(&bufferPool.full, 0, 0);
    pthread_mutex_init(&bufferPool.mutex, NULL);

    bufferPool.in = 0;
    bufferPool.out = 0;

    pthread_create(&tidA, NULL, producer, &producerId1);
    pthread_create(&tidB, NULL, producer, &producerId2);
    pthread_create(&tidC, NULL, consumer_sum, NULL);
    pthread_create(&tidD, NULL, consumer_product, NULL);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);
    pthread_join(tidC, NULL);
    pthread_join(tidD, NULL);

    pthread_mutex_destroy(&bufferPool.mutex);
    sem_destroy(&bufferPool.empty);
    sem_destroy(&bufferPool.full);

    return 0;
}

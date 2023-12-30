#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5 // 定义缓冲区大小
#define NUMBERS 10 // 定义每个文件读取的数字数量

// 定义缓冲池结构
typedef struct {
    int buffer[BUFFER_SIZE]; // 缓冲区数组
    int in, out; // 缓冲区的输入和输出索引

    sem_t empty, full; // 信号量：empty 表示空闲位置数量，full 表示已占用位置数量

    pthread_mutex_t mutex; // 互斥锁，用于保护对缓冲区的访问
} BufferPool;

BufferPool bufferPool; // 全局缓冲池实例

// 生产者线程函数
void *producer(void *param) {
    int producerId = *((int *)param); // 获取生产者ID
    FILE *file = fopen(producerId == 0 ? "data1.txt" : "data2.txt", "r"); // 根据ID打开对应的文件

    int number;
    for (int i = 0; i < NUMBERS; i++) {
        fscanf(file, "%d", &number); // 从文件读取数字

        sem_wait(&bufferPool.empty); // 等待缓冲区有空闲位置
        pthread_mutex_lock(&bufferPool.mutex); // 加锁

        bufferPool.buffer[bufferPool.in] = number; // 将数字放入缓冲区
        bufferPool.in = (bufferPool.in + 1) % BUFFER_SIZE; // 更新输入索引

        pthread_mutex_unlock(&bufferPool.mutex); // 解锁
        sem_post(&bufferPool.full); // 通知缓冲区有新数据
    }

    fclose(file); // 关闭文件
    return NULL;
}

// 消费者线程函数（求和）
void *consumer_sum(void *param) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int num1, num2;

        // 获取第一个数字
        sem_wait(&bufferPool.full); // 等待缓冲区有数据
        pthread_mutex_lock(&bufferPool.mutex); // 加锁
        num1 = bufferPool.buffer[bufferPool.out]; // 取出数字
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE; // 更新输出索引
        pthread_mutex_unlock(&bufferPool.mutex); // 解锁
        sem_post(&bufferPool.empty); // 通知缓冲区有空闲位置

        // 获取第二个数字
        sem_wait(&bufferPool.full); // 等待缓冲区有数据
        pthread_mutex_lock(&bufferPool.mutex); // 加锁
        num2 = bufferPool.buffer[bufferPool.out]; // 取出数字
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE; // 更新输出索引
        pthread_mutex_unlock(&bufferPool.mutex); // 解锁
        sem_post(&bufferPool.empty); // 通知缓冲区有空闲位置

        printf("Sum: %d + %d = %d\n", num1, num2, num1 + num2); // 打印和
    }
    return NULL;
}

// 消费者线程函数（求积）
void *consumer_product(void *param) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int num1, num2;

        // 获取第一个数字
        sem_wait(&bufferPool.full); // 等待缓冲区有数据
        pthread_mutex_lock(&bufferPool.mutex); // 加锁
        num1 = bufferPool.buffer[bufferPool.out]; // 取出数字
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE; // 更新输出索引
        pthread_mutex_unlock(&bufferPool.mutex); // 解锁
        sem_post(&bufferPool.empty); // 通知缓冲区有空闲位置

        // 获取第二个数字
        sem_wait(&bufferPool.full); // 等待缓冲区有数据
        pthread_mutex_lock(&bufferPool.mutex); // 加锁
        num2 = bufferPool.buffer[bufferPool.out]; // 取出数字
        bufferPool.out = (bufferPool.out + 1) % BUFFER_SIZE; // 更新输出索引
        pthread_mutex_unlock(&bufferPool.mutex); // 解锁
        sem_post(&bufferPool.empty); // 通知缓冲区有空闲位置

        printf("Product: %d * %d = %d\n", num1, num2, num1 * num2); // 打印积
    }
    return NULL;
}

// 主函数
int main() {
    pthread_t tidA, tidB, tidC, tidD; // 线程标识符
    int producerId1 = 0, producerId2 = 1; // 生产者ID

    // 初始化信号量和互斥锁
    sem_init(&bufferPool.empty, 0, BUFFER_SIZE);
    sem_init(&bufferPool.full, 0, 0);
    pthread_mutex_init(&bufferPool.mutex, NULL);

    bufferPool.in = 0; // 初始化输入索引
    bufferPool.out = 0; // 初始化输出索引

    // 创建线程
    pthread_create(&tidA, NULL, producer, &producerId1);
    pthread_create(&tidB, NULL, producer, &producerId2);
    pthread_create(&tidC, NULL, consumer_sum, NULL);
    pthread_create(&tidD, NULL, consumer_product, NULL);

    // 等待线程结束
    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);
    pthread_join(tidC, NULL);
    pthread_join(tidD, NULL);

    // 销毁互斥锁和信号量
    pthread_mutex_destroy(&bufferPool.mutex);
    sem_destroy(&bufferPool.empty);
    sem_destroy(&bufferPool.full);

    return 0;
}

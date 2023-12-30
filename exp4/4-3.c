#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double r;
    double i;
} Complex;

typedef struct {
    Complex a;
    Complex b;
    Complex result;
} ComplexArgs;

void *complexAdd(void *arg) {
    ComplexArgs *args = (ComplexArgs *)arg;
    args->result.r = args->a.r + args->b.r;
    args->result.i = args->a.i + args->b.i;
    return (void *)&args->result;
}

int main() {
    pthread_t thread_id;
    ComplexArgs args;
    Complex *result;

    args.a.r = 1.0; args.a.i = 2.0;
    args.b.r = 3.0; args.b.i = 4.0;

    if (pthread_create(&thread_id, NULL, complexAdd, &args) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    if (pthread_join(thread_id, (void **)&result) != 0) {
        perror("Failed to join thread");
        return EXIT_FAILURE;
    }

    printf("Result: %f + %fi\n", result->r, result->i);

    return EXIT_SUCCESS;
}

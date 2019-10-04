#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int a;
pthread_mutex_t mutex;

void *race(void* arg)
{
    int numIters = *(int*)arg, i;

    for (i = 0; i < numIters; ++i)
    {
        pthread_mutex_lock(&mutex);
        a += 2;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void getArgs(int argc, char **argv, int* N, int* P)
{
    if (argc < 3)
    {
        printf("Not enough paramters: ./program N P\n");
        exit(1);
    }

    *N = atoi(argv[1]);
    *P = atoi(argv[2]);
}


int main(int argc, char **argv)
{
    int i, N, P;

    getArgs(argc, argv, &N, &P);
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid[P];
    int thread_id[P], iters[P];

    for (i = 0; i < P; i++)
    {
        thread_id[i] = i;
    }

    for (i = 0; i < P - 1; i++)
    {
        iters[i] = ceil((double)N / P);
        pthread_create(&tid[i], NULL, race, iters + i);
    }
    iters[P - 1] = N - ceil((double)N / P) * (P - 1);
    pthread_create(&tid[P - 1], NULL, race, iters + P - 1);

    for (i = 0; i < P; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("%d\n", a);

    pthread_mutex_destroy(&mutex);

    return 0;
}

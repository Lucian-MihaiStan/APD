#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define NUM_ITERS       (1000)

int a;
pthread_barrier_t b;

void *race(void* arg)
{
    int i;

    for (i = 0; i < NUM_ITERS; ++i)
    {
        pthread_barrier_wait(&b);
        a += 2;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int i, P = 2;

    pthread_t tid[P];

    pthread_barrier_init(&b, NULL, 2);

    for (i = 0; i < P; i++)
    {
        pthread_create(&tid[i], NULL, race, NULL);
    }

    for (i = 0; i < P; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Teoretic minimul ar fi 2:
    // fiecare a += 2 se executa concomitent, la fiecare iteratie
    // Acest lucru e extrem de improbabil
    printf("%d\n", a);

    return 0;
}

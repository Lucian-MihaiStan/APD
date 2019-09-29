#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>

#define LOOP_LIMIT         (10000)

void* allCores(void *var)
{
    int i, j, k;
    u_int64_t res;

    for (i = 0; i < LOOP_LIMIT; ++i)
    {
        for (j = 0; j < LOOP_LIMIT; ++j)
        {
            for (k = 0; k < LOOP_LIMIT; ++k)
            {
                res = sqrt(ULONG_MAX);
            }
        }
    }
}

int main(int argc, char **argv)
{
    int P = sysconf(_SC_NPROCESSORS_ONLN);
    int i;

    pthread_t tid[P];

    for (i = 0; i < P; i++)
    {
        pthread_create(&(tid[i]), NULL, allCores, NULL);
    }

    for (i = 0; i < P; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

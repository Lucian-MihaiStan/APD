#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

/**
 * @author cristian.chilipirea
 * 
 */
#define MEMSET_VAL      (0xff)
#define MIN(x, y)       (((x) < (y)) ? (x) : (y))

int NReps;
int printLevel;
int N;
int P;
char* v;
int* threadIds;
pthread_t* threads;

void getArgs(int argc, char** argv)
{
    if (argc < 5)
    {
        printf("Not enough paramters: ./program N NReps P printLevel\nprintLevel: 0=no, 1=some, 2=verbose\n");
        exit(1);
    }

    N           = atoi(argv[1]);
    NReps       = atoi(argv[2]);
    P           = atoi(argv[3]);
    printLevel  = atoi(argv[4]);
}

void init()
{
    v = malloc(N * sizeof(*v));
    if (v == NULL)
    {
        printf("Malloc failed for the buffer.");
        exit(1);
    }

    threads = malloc(P * sizeof(*threads));
    if (threads == NULL)
    {
        printf("Malloc failed for threads.\n");
        
        free(v);
        
        exit(1);
    }

    threadIds = malloc(P * sizeof(*threadIds));
    if (threadIds == NULL)
    {
        printf("Malloc failed for thread ids.\n");
        
        free(v);
        free(threads);
        
        exit(1);
    }
}

void finalise()
{
    if (v != NULL)
    {
        free(v);
    }

    if (threads != NULL)
    {
        free(threads);
    }

    if (threadIds != NULL)
    {
        free(threadIds);
    }
}

void printPartial()
{
    int i, allCorrect = 1;

    for (i = 0; i != N; ++i)
    {
        if (v[i] != (char)MEMSET_VAL)
        {
            printf(
                "Element at position %d is 0x%x; it should be 0x%x\n",
                i,
                v[i],
                MEMSET_VAL
            );

            allCorrect = 0;
        }
    }

    if (allCorrect)
    {
        printf("Memset performed correctly.\n");
    }
}

void printAll()
{
    int i;

    for(i = 0; i != N; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void print()
{
    if (printLevel == 0)
    {
        return;
    } else if (printLevel == 1)
    {
        printPartial();
    } else
    {
        printAll();
    }
}

void* parMemset(void* arg)
{
    int i, j;

    int tid     = *(int*)arg;

    int start   = tid * ceil((double)N / P);
    int end     = MIN(N, (tid + 1) * ceil((double)N / P));

    for (i = 0; i != NReps; ++i)
    {
        memset(v + start, 0xff, (end - start) * sizeof(*v));
    }
}

int main(int argc, char** argv)
{
    int i;

    getArgs(argc, argv);
    init();

    for (i = 0; i != P; ++i)
    {
        threadIds[i] = i;

        pthread_create(threads + i, NULL, parMemset, threadIds + i);
    }

    for (i = 0; i != P; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    print();

    finalise();

    return 0;
}
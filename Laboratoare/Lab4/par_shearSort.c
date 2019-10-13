#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int P;
int printLevel;
int N;
int** m;
int** mT;
int* threadId;
pthread_t* tid;
pthread_barrier_t barrier;

int cmp(const void *a, const void *b)
{
    // DO NOT MODIFY
    int A = *(int*)a;
    int B = *(int*)b;
    
    return A - B;
}

void getArgs(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Not enough paramters: ./program N P printLevel\nprintLevel: 0=no, 1=some, 2=verbose\n");
        exit(1);
    }

    N           = atoi(argv[1]);
    P           = atoi(argv[2]);
    printLevel  = atoi(argv[3]);
}

void init()
{
    int i, j;

    m           = malloc(sizeof(int*) * N);
    mT          = malloc(sizeof(int*) * N);
    tid         = malloc(sizeof(pthread_t) * P);
    threadId    = malloc(sizeof(int) * P);
    
    pthread_barrier_init(&barrier, NULL, P);

    if (m == NULL || tid == NULL || threadId == NULL || mT == NULL)
    {
        printf("malloc failed!");
        exit(1);
    }

    // generate the vector v with random values
    // DO NOT MODIFY
    srand(42);
    
    for (i = 0; i < N; i++)
    {
        mT[i]   = malloc(sizeof(int) * N);
        m[i]    = malloc(sizeof(int) * N);

        if (m[i] == NULL || mT[i] == NULL)
        {
            printf("malloc failed!");
            exit(1);
        }
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; ++j)
        {
            m[i][j] = mT[j][i] = rand() % (N * N);
        }
    }
}

void destroy(void)
{
    int i;

    free(tid);
    free(threadId);

    for (i = 0; i < N; i++)
    {
        free(mT[i]);
        free(m[i]);
    }

    free(m);
    free(mT);

    pthread_barrier_destroy(&barrier);
}

void printAll()
{
    int i, j, start, end, step;

    for (i = 0; i < N; ++i)
    {
        if (i & 1)
        {
            start = N - 1;
            end = -1;
            step = -1;
        } else
        {
            start = 0;
            end = N;
            step = 1;
        }

        for (j = start; j != end; j += step)
        {
            printf("%d ", m[i][j]);
        }
    }
    printf("\n");
}

void printPartial()
{
    printAll();
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

int cmpEven(const void* p1, const void* p2)
{
    int num1 = *(int*)p1;
    int num2 = *(int*)p2;

    return num1 - num2;
}

int cmpOdd(const void* p1, const void* p2)
{
    int num1 = *(int*)p1;
    int num2 = *(int*)p2;

    return num2 - num1;
}

void sort_rows(int start, int end)
{
    int i, j;

    for (i = start; i < end; ++i)
    {
        if (i & 1)
        {
            qsort(m[i], N, sizeof(int), cmpOdd);
        } else
        {
            qsort(m[i], N, sizeof(int), cmpEven);
        }

        for (j = 0; j < N; ++j)
        {
            mT[j][i] = m[i][j];
        }
    }
}

void sort_cols(int start, int end)
{
    int i, j;

    for (i = start; i < end; ++i)
    {
        qsort(mT[i], N, sizeof(int), cmpEven);

        for (j = 0; j < N; ++j)
        {
            m[j][i] = mT[i][j];
        }
    }
}

void* shearSort(void* args)
{
    int id      = *(int*)args;
    int lim     = log2(N) + 1;
    int start   = id * ceil((double)N / P);
    int end     = MIN(N, (id + 1) * ceil((double)N / P));

    while (lim--)
    {
        sort_rows(start, end);
        pthread_barrier_wait(&barrier);
        sort_cols(start, end);
        pthread_barrier_wait(&barrier);
    }

}

int main(int argc, char *argv[])
{
    int i;
    getArgs(argc, argv);
    init();

    for (i = 0; i < P; ++i)
    {
        threadId[i] = i;
        pthread_create(tid + i, NULL, shearSort, threadId + i);
    }

    for (i = 0; i < P; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    print();
    destroy();

    return 0;
}
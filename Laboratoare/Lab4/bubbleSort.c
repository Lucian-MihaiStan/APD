#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int sorted;
int P;
int printLevel;
int N;
int* v;
int* vQSort;
int* threadId;
pthread_t* tid;

pthread_barrier_t barrier;

void* bubbleSort(void* arg)
{
    int i, aux;

    int id         = *(int*)arg;
    int srt         = 0;

    int start       = id * ceil((double)N / P);
    int end         = MIN(N, (id + 1) * ceil((double)N / P));

    int startEven   = start & 1 ? start + 1 : start;
    int endEven     = end & 1 ? end : end + 1;
    endEven         = MIN(N - 1, endEven);

    int startOdd    = start & 1 ? start : start + 1;
    int endOdd      = end & 1 ? end + 1 : end;
    endOdd          = MIN(N - 1, endOdd);

    while (!sorted)
    {
        pthread_barrier_wait(&barrier);
        sorted = 1;
        srt = 1;

        for (i = startEven; i < endEven; i += 2)
        {
            if (v[i] > v[i + 1])
            {
                aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                
                srt = 0;
            }
        }

        pthread_barrier_wait(&barrier);

        for (i = startOdd; i < endOdd; i += 2)
        {
            if (v[i] > v[i + 1])
            {
                aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                
                srt = 0;
            }
        }

        if (!srt)
        {
            sorted = 0;
        }

        pthread_barrier_wait(&barrier);
    }
}

void compareVectors(int* a, int* b)
{
    // DO NOT MODIFY
    int i;

    for (i = 0; i < N; i++)
    {
        if (a[i] != b[i])
        {
            printf("Sorted incorrectly\n");
            return;
        }
    }
    printf("Sorted correctly\n");
}

void displayVector(int * v)
{
    // DO NOT MODIFY
    int i;
    int displayWidth = 2 + log10(v[N - 1]);

    for (i = 0; i < N; i++)
    {
        printf("%*i", displayWidth, v[i]);
        
        if(!((i + 1) % 20))
        {
            printf("\n");
        }
    }
    printf("\n");
}

int cmp(const void* a, const void* b)
{
    // DO NOT MODIFY
    int A = *(int*)a;
    int B = *(int*)b;
    
    return A - B;
}

void getArgs(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("Not enough paramters: ./program N P printLevel\nprintLevel: 0=no, 1=some, 2=verbose\n");
        exit(1);
    }

    N = atoi(argv[1]);
    P = atoi(argv[2]);
    printLevel = atoi(argv[3]);
}

void init()
{
    int i;

    v           = malloc(sizeof(int) * N);
    vQSort      = malloc(sizeof(int) * N);
    tid         = malloc(sizeof(pthread_t) * P);
    threadId    = malloc(sizeof(int) * P);

    if (v == NULL || vQSort == NULL || tid == NULL || threadId == NULL)
    {
        printf("malloc failed!");
        exit(1);
    }

    // generate the vector v with random values
    // DO NOT MODIFY
    srand(42);
    
    for (i = 0; i < N; i++)
    {
        v[i] = rand() % N;
    }

    pthread_barrier_init(&barrier, NULL, P);
}

void destroy(void)
{
    free(v);
    free(vQSort);
    free(tid);
    free(threadId);
    pthread_barrier_destroy(&barrier);
}

void printPartial()
{
    compareVectors(v, vQSort);
}

void printAll()
{
    displayVector(v);
    displayVector(vQSort);
    compareVectors(v, vQSort);
}

void print()
{
    if (printLevel == 0)
    {
        return;
    }

    if (printLevel == 1)
    {
        printPartial();
    } else
    {
        printAll();
    }
}

int main(int argc, char *argv[])
{
    int i, j, aux;
    getArgs(argc, argv);
    init();

    // make copy to check it against qsort
    // DO NOT MODIFY
    for(i = 0; i < N; i++)
    {
        vQSort[i] = v[i];
    }
    qsort(vQSort, N, sizeof(int), cmp);

    // sort the vector v
    // PARALLELIZE ME

    for (i = 0; i < P; ++i)
    {
        threadId[i] = i;
        pthread_create(tid + i, NULL, bubbleSort, threadId + i);
    }

    for (i = 0; i < P; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    print();
    destroy();

    return 0;
}

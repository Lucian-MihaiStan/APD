#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int printLevel;
int N;
int P;
int* v;
int* vNew;
pthread_t* tid;
int* threadID;
pthread_barrier_t barrier;
int changed;

void merge(int source[], int start, int mid, int end, int destination[]) {
    // DO NOT MODIFY
    int iA = start;
    int iB = mid;
    int i;

    for (i = start; i < end; i++)
    {
        if (end == iB || (iA < mid && source[iA] <= source[iB]))
        {
            destination[i] = source[iA];
            iA++;
        } else
        {
            destination[i] = source[iB];
            iB++;
        }
    }
}

void compareVectors(int * a, int * b) {
    // DO NOT MODIFY
    int i;

    for (i = 0; i < N; i++)
    {
        if (a[i]!=b[i])
        {
            printf("Sorted incorrectly\n");
            return;
        }
    }
    printf("Sorted correctly\n");
}

void displayVector(int* v) {
    // DO NOT MODIFY
    int i;

    for (i = 0; i < N; i++)
    {
        printf("%d ", v[i]);
        
        if(!((i + 1) % 20))
        {
            printf("\n");
        }
    }
    printf("\n");
}

int cmp(const void* a, const void* b) {
    // DO NOT MODIFY
    int A = *(int*)a;
    int B = *(int*)b;
    
    return A - B;
}

void getArgs(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Not enough paramters: ./program N printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
        exit(1);
    }

    N           = atoi(argv[1]);
    P           = atoi(argv[2]);
    printLevel  = atoi(argv[3]);
}

void init()
{
    int i;

    v           = malloc(sizeof(int) * N);
    vNew        = malloc(sizeof(int) * N);
    tid         = malloc(sizeof(pthread_t) * P);
    threadID    = malloc(sizeof(int) * P);

    if (v == NULL || vNew == NULL || tid == NULL || threadID == NULL)
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
    free(vNew);
    free(tid);
    free(threadID);
    pthread_barrier_destroy(&barrier);
}

void printPartial()
{
    displayVector(v);
}

void printAll()
{
    displayVector(v);
}

void print()
{
    if (printLevel == 0)
    {
        return;
    }

    if(printLevel == 1)
    {
        printPartial();
    } else
    {
        printAll();
    }
}

void* mergeSort(void* arg)
{
    int tid     = *(int*)arg;
    int start   = tid * ceil((double)N / P);
    int end     = MIN(N, (tid + 1) * ceil((double)N / P));

    int i, width, *aux;

    for (width = 1; width < N; width <<= 1)
    {
        if (end - start == width)
        {
            if ((end / width) & 1)
            {
                end -= width;
            } else
            {
                start -= width;
            }
        }

        for (i = start; i < end; i += (width * 2))
        {
            merge(v, i, i + width, i + 2 * width, vNew);
        }

        pthread_barrier_wait(&barrier);

        if (changed == tid)
        {
            aux = v;
            v = vNew;
            vNew = aux;
        }           

        pthread_barrier_wait(&barrier);
    }
}

int main(int argc, char *argv[])
{
    int i;
    getArgs(argc, argv);
    init();

    for (i = 0; i != P; ++i)
    {
        threadID[i] = i;
        pthread_create(tid + i, NULL, mergeSort, threadID + i);
    }

    for (i = 0; i != P; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    print();
    destroy();

    return 0;
}

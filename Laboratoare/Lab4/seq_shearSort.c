#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int printLevel;
int** m;

void getArgs(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Not enough paramters: ./program N printLevel\nprintLevel: 0=no, 1=some, 2=verbose\n");
        exit(1);
    }

    N           = atoi(argv[1]);
    printLevel  = atoi(argv[2]);
}

void init()
{
    m = malloc(sizeof(int*) * N);

    if (m == NULL)
    {
        printf("malloc failed!");
        exit(1);
    }

    int i, j;
    srand(42);

    for(i = 0; i < N; i++)
    {
        m[i] = malloc(sizeof(int) * N);

        if (m[i] == NULL)
        {
            printf("malloc failed!");
            exit(1);
        }
    }

    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; j++)
        {
            m[i][j] = rand() % (N * N);
        }
    }
}

void destroy()
{
    int i;

    for (i = 0; i < N; ++i)
    {
        free(m[i]);
    }

    free(m);
}

void printAll(int** x)
{
    int i, j, start, end, step;
    
    for (i = 0; i < N; i++)
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
            printf("%i ", x[i][j]);
        }
    }
    printf("\n");
}

void printPartial(int** x)
{
    printAll(x);
}

void print(int** x)
{
    if (printLevel == 0)
    {
        return;
    } else if (printLevel == 1)
    {
        printPartial(x);
    } else 
    {
        printAll(x);
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

void sort_rows(int** x)
{
    int i;

    for (i = 0; i < N; ++i)
    {
        if (i & 1)
        {
            qsort(x[i], N, sizeof(int), cmpOdd);
        } else
        {
            qsort(x[i], N, sizeof(int), cmpEven);
        }
    }
}

void simple_sort(int** x)
{
    int i;

    for (i = 0; i < N; ++i)
    {
        qsort(x[i], N, sizeof(int), cmpEven);
    }
}

void transpose(int** x)
{
    int i, j, tmp;

    for (i = 0; i < N; ++i)
    {
        for (j = i; j < N; ++j)
        {
            tmp = x[i][j];
            x[i][j] = x[j][i];
            x[j][i] = tmp; 
        }
    }
}

void sort_cols(int** x)
{
    transpose(x);
    simple_sort(x);
    transpose(x);
}

void shearSort(void)
{
    int i, lim = log2(N) + 1;

    while (lim--)
    {
        sort_rows(m);
        sort_cols(m);
    }
}

int main(int argc, char** argv)
{
    getArgs(argc, argv);
    init();
    
    shearSort();

    print(m);
    destroy();

    return 0;
}
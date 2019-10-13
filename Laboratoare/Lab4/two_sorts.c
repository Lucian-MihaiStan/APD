#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N = 10;
int printLevel = 2;
int** a;
int** b;

void init()
{
    a = malloc(sizeof(int*) * N);
    b = malloc(sizeof(int*) * N);

    if (a == NULL || b == NULL)
    {
        printf("malloc failed!");
        exit(1);
    }

    int i, j;

    for(i = 0; i < N; i++)
    {
        a[i] = malloc(sizeof(int) * N);
        b[i] = malloc(sizeof(int) * N);

        if (a[i] == NULL || b[i] == NULL)
        {
            printf("malloc failed!");
            exit(1);
        }

        for (j = 0; j < N; j++)
        {
            b[i][j] = a[i][j] = rand() % 100;
        }
    }
}

void destroy()
{
    int i;

    for (i = 0; i < N; ++i)
    {
        free(a[i]);
        free(b[i]);
    }

    free(a);
    free(b);
}

void printAll(int** x)
{
    int i, j;
    
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%i ", x[i][j]);
        }
        printf("\n");
    }
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

int cmp(const void* p1, const void* p2)
{
    int num1 = *(int*)p1;
    int num2 = *(int*)p2;

    return num1 - num2;
}

void sort_rows(int** x)
{
    int i;

    for (i = 0; i < N; ++i)
    {
        qsort(x[i], N, sizeof(int), cmp);
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
    sort_rows(x);
    transpose(x);
}

int main(void)
{
    srand(time(NULL));
    init();

    printf("Oringinal matrix:\n");
    print(a);
    printf("\n");
    
    printf("Sorted rows:\n");
    sort_rows(a);
    print(a);
    printf("\n");

    printf("Sorted columns:\n");
    sort_cols(b);
    print(b);

    destroy();

    return 0;
}
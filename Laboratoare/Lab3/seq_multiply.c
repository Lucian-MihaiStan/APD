#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps;
int printLevel;
int N;
int** a;
int** b;
int** c;

void getArgs(int argc, char **argv)
{
    if(argc < 4)
    {
        printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
        exit(1);
    }
    
    N = atoi(argv[1]);
    NReps = atoi(argv[2]);
    printLevel = atoi(argv[3]);
}

void init()
{
    a = malloc(sizeof(int *) * N);
    b = malloc(sizeof(int *) * N);
    c = malloc(sizeof(int *) * N);

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("malloc failed!");
        exit(1);
    }

    int i, j;

    for(i = 0; i < N; i++)
    {
        a[i] = malloc(sizeof(int) * N);
        b[i] = malloc(sizeof(int) * N);
        c[i] = malloc(sizeof(int) * N);

        if (a[i] == NULL || b[i] == NULL || c[i] == NULL)
        {
            printf("malloc failed!");
            exit(1);
        }
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (i <= j)
            {
                a[i][j] = 1;
                b[i][j] = 1;
            } else {
                a[i][j] = 0;
                b[i][j] = 0;
            }
        }
    }
}

void printAll()
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%i\t",c[i][j]);
        }
        printf("\n");
    }
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

int main(int argc, char *argv[])
{
    int i, j, k, t, tmp;
    getArgs(argc, argv);
    init();

    for (t = 0; t < NReps; ++t)
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                tmp = 0;

                for(k = 0; k < N; k++)
                {
                    tmp += a[i][k] * b[k][j];
                }

                c[i][j] = tmp;
            }
        }
    }
    

    print();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N           (100)
#define MASTER      (0)

/**
 * @author cristian.chilipirea
 *
 */
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

void displayVector(int* v)
{
    // DO NOT MODIFY
    int i;
    
    for (i = 0; i < N; i++)
    {
        printf("%d ", v[i]);
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
 
int main(int argc, char* argv[])
{
    int rank;
    int nProcesses;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    int nextProc    = rank + 1;
    int prevProc    = rank - 1;
    int len         = N - rank;

    int* v, i;
    
    // This code is run by a single process
    if (rank == 0)
    {        
        v           = malloc(sizeof(*v) * N);
        int* vQSort = malloc(sizeof(*vQSort) * N);

        // generate the vector v with random values
        // DO NOT MODIFY
        srand(42);

        for (i = 0; i != N; ++i)
        {
            v[i] = rand() % N;
        }

        // make copy to check it against qsort
        // DO NOT MODIFY
        for (i = 0; i != N; ++i)
        {
            vQSort[i] = v[i];
        }
        qsort(vQSort, N, sizeof(int), cmp);

        len = N - rank;

        for (i = 1; i != len; ++i)
        {
            if (v[0] > v[i])
            {
                MPI_Send(v, 1, MPI_INT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
                v[0] = v[i];
            } else
            {
                MPI_Send(&v[i], 1, MPI_INT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
            }
        }

        MPI_Gather(v, 1, MPI_INT, &v[0], 1, MPI_INT, MASTER, MPI_COMM_WORLD);

        //displayVector(v);
        compareVectors(v, vQSort);

        free(vQSort);
    } else
    {
        v = malloc(sizeof(*v));  // alftfel da seg fault... fmm MPI
        int recvNum;
        
        MPI_Recv(
            v,
            1,
            MPI_INT,
            prevProc,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        for (i = 1; i != len; ++i)
        {
            MPI_Recv(
                &recvNum,
                1,
                MPI_INT,
                prevProc,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            if (*v > recvNum)
            {
                MPI_Send(v, 1, MPI_INT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
                *v = recvNum;
            } else
            {
                MPI_Send(&recvNum, 1, MPI_INT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
            }
        }

        MPI_Gather(v, 1, MPI_INT, v, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    }

    free(v);

    MPI_Finalize();

    return 0;
}
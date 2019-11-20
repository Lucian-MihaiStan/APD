#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define START       (0)

int main (int argc, char* argv[])
{
    int i, rank, numProc, step, nextProc, number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    if (rank == START)
    {
        number  = 42;
        step    = 1;
    } else
    {
        MPI_Recv(
            &number,
            1,
            MPI_INT,
            MPI_ANY_SOURCE,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        printf("Process %d received number %d.\n", rank, number);

        step = 1 << ((int)log2(rank) + 1);
    }

    for (i = rank + step; i < numProc; i += step, step <<= 1)
    {
        MPI_Send(&number, 1, MPI_INT, i, MPI_TAG_UB, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
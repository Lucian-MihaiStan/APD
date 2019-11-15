#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define INCREMENT   (2)

int main (int argc, char* argv[])
{
    int rank, numProc, prevProc, nextProc, number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    if (rank == 0)
    {
        number      = 1;
        prevProc    = numProc - 1;

        printf("Process %d: number = %d\n", rank, number);
        sleep(1);

        MPI_Send(&number, 1, MPI_INT, 1, MPI_TAG_UB, MPI_COMM_WORLD);
    } else
    {
        prevProc = rank - 1;
    }

    nextProc = (rank + 1) % numProc;

    while (1)
    {
        MPI_Recv(&number, 1, MPI_INT, prevProc, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        number += 2;
        
        printf("Process %d: number = %d\n", rank, number);
        sleep(1);

        MPI_Send(&number, 1, MPI_INT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
    }

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ELEMS   (1 << 30)
#define MASTER      (0)

int main (int argc, char* argv[])
{
    int i, rank, numProc, step;
    uint64_t recvSum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int* v          = NULL;
    uint64_t sum    = 0;
    int numElems    = NUM_ELEMS / numProc;

    if (rank == MASTER)
    {
        v = malloc(NUM_ELEMS * sizeof(*v));
    } else
    {
        v = malloc(numElems * sizeof(*v));
    }

    if (v == NULL)
    {
        fprintf(stderr, "Unable to allocate the buffer.\n");
        return 0;
    }    

    if (rank == MASTER)
    {
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            v[i] = i;
        }
    }

    MPI_Scatter(
        v,
        numElems,
        MPI_INT,
        v,
        numElems,
        MPI_INT,
        MASTER,
        MPI_COMM_WORLD
    );

    for (i = 0; i < numElems; ++i)
    {
        sum += v[i];
    }

    for (step = 2; step <= numProc; step <<= 1)
    {
        if (!(rank % step))
        {
            MPI_Recv(
                &recvSum,
                1,
                MPI_LONG_LONG,
                rank + (step >> 1),
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            sum += recvSum;
        } else if (!(rank % (step >> 1)))
        {
            MPI_Send(
                &sum,
                1,
                MPI_LONG_LONG,
                rank - (step >> 1),
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == MASTER)
    {
        printf(
            "calculated sum = %lu; true sum = %lu\n",
            sum, 1LU * NUM_ELEMS * (NUM_ELEMS - 1) / 2);
    }

    MPI_Finalize();

    return 0;
}
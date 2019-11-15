#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMS   (100)

int main (int argc, char* argv[])
{
    int* v = malloc(NUM_ELEMS * sizeof(*v));

    if (v == NULL)
    {
        fprintf(stderr, "Unable to allocate the buffer.\n");
        return 0;
    }

    int i, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            v[i] = i;
        }

        MPI_Send(v, NUM_ELEMS, MPI_INT, 1, MPI_TAG_UB, MPI_COMM_WORLD);
    } else
    {
        MPI_Status status; 

        MPI_Recv(v, NUM_ELEMS, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        printf(
            "Received message from process %d, with tag %d:\n",
            status.MPI_SOURCE,
            status.MPI_TAG);

        for (i = 0; i != NUM_ELEMS; ++i)
        {
            printf("%d ", v[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
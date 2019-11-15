#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMS   (1000)

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
    }

    MPI_Bcast(v, NUM_ELEMS, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        int correct = 1;

        for (i = 0; i != NUM_ELEMS; ++i)
        {
            if (v[i] != i)
            {
                printf(
                    "Process %d received incorrect data: v[%d] = %d\n",
                    rank,
                    i,
                    v[i]
                );

                correct = 0;

                break;
            }
        }

        if (correct)
        {
            printf("Process %d received correct data\n", rank);
        }
    }

    MPI_Finalize();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMS   (10000)
#define INCREMENT   (42)

int main (int argc, char* argv[])
{
    int* v = malloc(NUM_ELEMS * sizeof(*v));

    if (v == NULL)
    {
        fprintf(stderr, "Unable to allocate the buffer.\n");
        return 0;
    }

    int i, rank, numProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    if (rank == 0)
    {
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            v[i] = i;
        }
    }

    int numElems = NUM_ELEMS / numProc;

    MPI_Scatter(v, numElems, MPI_INT, v, numElems, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < numElems; ++i)
    {
        v[i] += INCREMENT;
    }

    MPI_Gather(v, numElems, MPI_INT, v, numElems, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Final array values:\n");

        for (i = 0; i < NUM_ELEMS; ++i)
        {
            printf("%d ", v[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MASTER      (0)
#define NUM_ELEMS   (10000)

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 2 ./a.out
 */
int main(int argc, char * argv[])
{
    srand(time(NULL));

    int* v = malloc(NUM_ELEMS * sizeof(*v));
    int i, rank, nProcesses;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    memset(v, 0xff, NUM_ELEMS * sizeof(*v));

    if (rank == MASTER)
    {
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            v[i] = i;
        }

        MPI_Send(v, NUM_ELEMS, MPI_INT, 1, MPI_TAG_UB, MPI_COMM_WORLD);
        printf("Got here! MPI_Send is non-blocking because there is no one to do a receive.\n");
    } else
    {
        MPI_Recv(v, NUM_ELEMS, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Received: ");
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            printf("%d ", v[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
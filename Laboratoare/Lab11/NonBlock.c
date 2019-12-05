#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MASTER      (0)
#define NUM_ELEMS   (10000)

int main(int argc, char* argv[])
{
    srand(time(NULL));

    int i, rank;
    int* buff = calloc(NUM_ELEMS, sizeof(*buff));

    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER)
    {
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            buff[i] = i;
        }

        MPI_Isend(buff, NUM_ELEMS, MPI_INT, 1, MPI_TAG_UB, MPI_COMM_WORLD, &request);
        // MPI_Wait(&request, &status);  // ca sa nu se strice bufferu'

        for (i = NUM_ELEMS; i != -1; --i)
        {
            buff[i] = -1;
        }
    } else
    {
        MPI_Irecv(buff, NUM_ELEMS, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);

        printf("Process 1 received:\n");
        for (i = 0; i != NUM_ELEMS; ++i)
        {
            printf("%d ", buff[i]);
        }
        printf("\n");
    }

    free(buff);
    
    MPI_Finalize();

    return 0;
}
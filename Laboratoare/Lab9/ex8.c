#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER  (1000)
#define NUM_SENDS   (10)

int main (int argc, char* argv[])
{
    srand(time(NULL));

    int i, rank, number, tag;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1)
    {
        MPI_Status status;

        for (i = 0; i != NUM_SENDS; ++i)
        {
            MPI_Recv(
                &number,
                1,
                MPI_INT,
                MPI_ANY_SOURCE,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );

            printf(
                "Process %d sent %d with tag %d\n",
                status.MPI_SOURCE,
                number,
                status.MPI_TAG
            );
        }
    } else
    {
        for (i = 0; i != NUM_SENDS; ++i)
        {
            number  = rand() % MAX_NUMBER;
            tag     = rand() % MAX_NUMBER;

            MPI_Send(&number, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER  (1000)

int main (int argc, char* argv[])
{
    int i, rank, number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 3)
    {
        MPI_Status status;

        for (i = 0; i != 3; ++i)
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

            printf("Process %d sent %d\n", status.MPI_SOURCE, number);
        }
    } else
    {
        number = rand() % MAX_NUMBER;  // TODO: cum fac ca fiecare proces sa aiba alt random?

        MPI_Send(&number, 1, MPI_INT, 3, MPI_TAG_UB, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
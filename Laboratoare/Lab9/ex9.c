#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PRECISION   (1000000)
#define NUM_POINTS  (10000000)
#define RADIUS      (1.0)

int main (int argc, char* argv[])
{
    int rank, numProc, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    srand(time(NULL));

    double pointX, pointY;
    int numPointsInCircle = 0;

    for (i = 0; i < NUM_POINTS; ++i)
    {
        pointX = (double)(rand() % (PRECISION * 2)) / PRECISION - RADIUS;
        pointY = (double)(rand() % (PRECISION * 2)) / PRECISION - RADIUS;

        if (pointX * pointX + pointY * pointY <= RADIUS)
        {
            ++numPointsInCircle;
        }
    }

    if (rank == 0)
    {
        int recvPoints;
        double pi;

        for (i = 0; i != numProc; ++i)
        {
            if (i != rank)
            {
                MPI_Recv(
                    &recvPoints,
                    1,
                    MPI_INT,
                    MPI_ANY_SOURCE,
                    MPI_ANY_TAG,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
                );

                numPointsInCircle += recvPoints;
            }
        }

        pi = 4.0 * numPointsInCircle / (NUM_POINTS * numProc);

        printf("PI = %lf\n", pi);
    } else
    {
        MPI_Send(&numPointsInCircle, 1, MPI_INT, 0, MPI_TAG_UB, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
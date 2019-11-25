#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER      (0)
#define MAX_X       (10)

/**
 * @author cristian.chilipirea
 *
 */
int main(int argc, char * argv[])
{
    int rank;
    int nProcesses;

    float coeff;
    float* a;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    float* v = malloc(3 * sizeof(*v));

    int nextProc = rank + 1, prevProc = rank - 1;

    // This code is run by a single process
    if (rank == MASTER)
    {
        int i, polynomialSize;
        float x;

        FILE* polFunctionFile = fopen("a2.txt", "rt");

        fscanf(polFunctionFile, "%i", &polynomialSize);
        a = malloc(polynomialSize * sizeof(*a));
        
        for (i = 0; i < polynomialSize; ++i)
        {
            fscanf(polFunctionFile, "%f", a + i);
        }

        fclose(polFunctionFile);

        MPI_Scatter(
            a,
            1,
            MPI_FLOAT,
            &coeff,
            1,
            MPI_FLOAT,
            MASTER,
            MPI_COMM_WORLD
        );

        v[0] = a[MASTER];
        v[2] = 1.f;

        for (x = 0; x < MAX_X; ++x)
        {
            v[1] = x;

            MPI_Send(v, 3, MPI_FLOAT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
        }

        free(a);
    } else
    {
        int i;

        MPI_Scatter(
            a,
            1,
            MPI_FLOAT,
            &coeff,
            1,
            MPI_FLOAT,
            MASTER,
            MPI_COMM_WORLD
        );

        for (i = 0; i != MAX_X; ++i)
        {
            MPI_Recv(
                v,
                3,
                MPI_FLOAT,
                prevProc,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            v[2] *= v[1];
            v[0] += coeff * v[2];

            if (nextProc == nProcesses)
            {
                printf("P(%f) = %f\n", v[1], v[0]);
            } else
            {
                MPI_Send(v, 3, MPI_FLOAT, nextProc, MPI_TAG_UB, MPI_COMM_WORLD);
            }
        }
    }

    free(v);
    
    MPI_Finalize();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define N           (10000)
#define MASTER      (0)

/**
 * @author cristian.chilipirea
 *
 */
void compareVectors(int* a, int* b)
{
    // DO NOT MODIFY
    int i;
    
    for (i = 0; i < N; i++)
    {
        if (a[i] != b[i])
        {
            printf("Sorted incorrectly\n");
            return;
        }
    }
    printf("Sorted correctly\n");
}

void displayVector(int* v)
{
    // DO NOT MODIFY
    int i;
    
    for (i = 0; i < N; i++)
    {
        printf("%d ", v[i]);
    }

    printf("\n");
}

int cmp(const void* a, const void* b)
{
    // DO NOT MODIFY
    int A = *(int*)a;
    int B = *(int*)b;
    
    return A - B;
}
 
int main(int argc, char* argv[])
{
    int rank;
    int nProcesses;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    int start   = rank * ceil((double)N / nProcesses);
    int end     = MIN(N, (rank + 1) * ceil((double)N / nProcesses));

    int* pos;
    int* v = malloc(sizeof(*v) * N);
    int i, j;
    
    // This code is run by a single process
    if (rank == MASTER)
    {
        // generate the vector v with random values
        // DO NOT MODIFY
        srand(42);

        for (i = 0; i != N; ++i)
        {
            v[i] = rand() % N;
        }

        pos = calloc(N, sizeof(*pos));
    } else
    {
        pos = calloc(end - start, sizeof(*pos));
    }

    MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    for (i = start; i != end; ++i)
    {
        for (j = 0; j != N; ++j)
        {
            if (v[j] < v[i] || (j < i && v[j] == v[i]))
            {
                ++pos[i - start];
            }
        }
    }

    MPI_Gather(
        pos,
        end - start,
        MPI_INT,
        pos,
        end - start,
        MPI_INT,
        MASTER,
        MPI_COMM_WORLD
    );

    if (rank == MASTER)
    {
        int* vQSort = malloc(sizeof(*vQSort) * N);
        int* w      = malloc(sizeof(*w) * N);

        // make copy to check it against qsort
        // DO NOT MODIFY
        for (i = 0; i != N; ++i)
        {
            vQSort[i] = v[i];
        }
        qsort(vQSort, N, sizeof(int), cmp);

        for (i = 0; i != N; ++i)
        {
            w[pos[i]] = v[i];
        }

        //displayVector(w);
        compareVectors(w, vQSort);

        free(vQSort);
        free(w);
    }

    free(v);
    free(pos);

    MPI_Finalize();

    return 0;
}
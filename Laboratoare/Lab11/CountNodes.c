#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 10 ./a.out
 */
int graph[][2] = {{ 0, 1 }, { 0, 4 }, { 0, 5 }, 
                  { 1, 0 }, { 1, 2 }, { 1, 6 }, 
                  { 2, 1 }, { 2, 3 }, { 2, 7 },
                  { 3, 2 }, { 3, 4 }, { 3, 8 },
                  { 4, 0 }, { 4, 3 }, { 4, 9 },
                  { 5, 0 }, { 5, 7 }, { 5, 8 },
                  { 6, 1 }, { 6, 8 }, { 6, 9 },
                  { 7, 2 }, { 7, 5 }, { 7, 9 },
                  { 8, 3 }, { 8, 5 }, { 8, 6 },
                  { 9, 4 }, { 9, 6 }, { 9, 7 }};

int* getNeighbours(int node, int* numNeighbours)
{
    int start, end, i;
    int* neighbours = NULL;
    int len         = sizeof(graph) / 8;

    for (start = 0; start != len && graph[start][0] != node; ++start);
    for (end = len - 1; end != -1 && graph[end][0] != node; --end);

    if (end >= start)
    {
        *numNeighbours  = (end - start + 1);
        neighbours      = malloc(*numNeighbours * sizeof(*neighbours));

        for (i = start; i <= end; ++i)
        {
            neighbours[i - start] = graph[i][1];
        }
    } else
    {
        *numNeighbours = 0;
    }

    return neighbours;
}

int findLeader(int rank, int numProc)
{
    int numNeighbours, i, j, leader, newLeader;
    int* neighbours;

    leader      = rank;
    neighbours  = getNeighbours(rank, &numNeighbours);

    for (i = 0; i != numProc; ++i)
    {
        for (j = 0; j != numNeighbours; ++j)
        {
            MPI_Send(
                &leader,
                1,
                MPI_INT,
                neighbours[j],
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );

            MPI_Recv(
                &newLeader,
                1,
                MPI_INT,
                MPI_ANY_SOURCE,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            leader = MAX(leader, newLeader);
        }
    }

    free(neighbours);

    return leader;
}

int main(int argc, char* argv[])
{
    int i, j, rank, numProc, leader;
    float num, newNum;
    int* neighbours;
    int numNeighbours;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    neighbours  = getNeighbours(rank, &numNeighbours);
    leader      = findLeader(rank, numProc);

    if (rank == leader)
    {
        num = 1.f;
    } else
    {
        num = 0.f;
    }

    for (i = 0; i != numProc; ++i)
    {
        for (j = 0; j != numNeighbours; ++j)
        {
            MPI_Sendrecv(
                &num,
                1,
                MPI_FLOAT,
                neighbours[j],
                MPI_TAG_UB,
                &newNum,
                1,
                MPI_FLOAT,
                MPI_ANY_SOURCE,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            num = (num + newNum) / 2.f;
        }
    }

    printf("Process %d says there are %d nodes\n", rank, (int)round(1.f / num));

    free(neighbours);
    MPI_Finalize();

    return 0;
}
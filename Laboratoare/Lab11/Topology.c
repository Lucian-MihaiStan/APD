#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NODES       (10)
#define INITIATOR       (0)

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

int* distributedSpanningTree(int rank)
{
    int i, j, numNeighbours;

    int* neighbours     = getNeighbours(rank, &numNeighbours);
    int* parents        = malloc(NUM_NODES * sizeof(*parents));
    int* recvParents    = malloc(NUM_NODES * sizeof(*recvParents));

    MPI_Status status;

    memset(parents, 0xff, NUM_NODES * sizeof(*parents));

    if (rank != INITIATOR)
    {
        MPI_Recv(
            parents,
            NUM_NODES,
            MPI_INT,
            MPI_ANY_SOURCE,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            &status
        );

        parents[rank] = status.MPI_SOURCE;

        printf("parent[%d] = %d\n", rank, parents[rank]);
    }

    for (i = 0; i != numNeighbours; ++i)
    {
        if (neighbours[i] != parents[rank])
        {
            MPI_Send(
                parents,
                NUM_NODES,
                MPI_INT,
                neighbours[i],
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }
    }

    for (i = 0; i != numNeighbours; ++i)
    {
        if (neighbours[i] != parents[rank])
        {
            MPI_Recv(
                recvParents,
                NUM_NODES,
                MPI_INT,
                neighbours[i],
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            for (j = 0; j != NUM_NODES; ++j)
            {
                if (parents[j] == -1)
                {
                    parents[j] = recvParents[j];
                }
            }
        }
    }

    if (rank != INITIATOR)
    {
        MPI_Send(
            parents,
            NUM_NODES,
            MPI_INT,
            parents[rank],
            MPI_TAG_UB,
            MPI_COMM_WORLD
        );

        MPI_Recv(
            parents,
            NUM_NODES,
            MPI_INT,
            MPI_ANY_SOURCE,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            &status
        );
    }

    for (i = 0; i != numNeighbours; ++i)
    {
        if (neighbours[i] != parents[rank])
        {
            MPI_Send(
                parents,
                NUM_NODES,
                MPI_INT,
                neighbours[i],
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }
    }

    if (rank == NUM_NODES - 1)
    {
        for (i = 0; i != NUM_NODES; ++i)
        {
            printf("parinte[%d] = %d\n", i, parents[i]);
        }
    }

    free(neighbours);
    free(recvParents);

    return parents;
}

int main(int argc, char* argv[])
{
    int rank;

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    distributedSpanningTree(rank);

    MPI_Finalize();

    return 0;
}
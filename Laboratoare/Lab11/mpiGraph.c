#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NODES       (10)

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

int* firstStepBFS(int node)
{
    int* visited    = malloc(NUM_NODES * sizeof(*visited));
    int* queue      = malloc(NUM_NODES * sizeof(*visited));
    int qStart      = 0;
    int qEnd        = -1;
    
    int i, crtNode, numNeighbours;
    int* neighbours = NULL;

    memset(visited, 0xff, NUM_NODES * sizeof(*visited));

    visited[node]   = node;
    neighbours      = getNeighbours(node, &numNeighbours);

    for (i = 0; i != numNeighbours; ++i)
    {
        queue[++qEnd] = visited[neighbours[i]] = neighbours[i];
    }

    while (qEnd >= qStart)
    {
        free(neighbours);

        crtNode     = queue[qStart++];
        neighbours  = getNeighbours(crtNode, &numNeighbours);

        for (i = 0; i != numNeighbours; ++i)
        {
            if (visited[neighbours[i]] == -1)
            {
                queue[++qEnd]           = neighbours[i];
                visited[neighbours[i]]  = visited[crtNode];
            }
        }
    }

    free (neighbours);
    free (queue);

    return visited;
}

int main(int argc, char* argv[])
{
    int* neighbours = NULL;
    int* visited    = NULL;

    int rank, nProcesses, numNeighbours, i;
    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    neighbours = getNeighbours(rank, &numNeighbours);

    printf("Process %d has neighbours: ", rank);
    for (i = 0; i != numNeighbours; ++i)
    {
        printf("%d ", neighbours[i]);
    }
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    visited = firstStepBFS(rank);

    printf("From process %d: ", rank);
    for (i = 0; i < NUM_NODES; ++i)
    {
        printf("to process %d: %d; ", i, visited[i]);
    }
    printf("\n");

    free(neighbours);
    free(visited);
    MPI_Finalize();

    return 0;
}
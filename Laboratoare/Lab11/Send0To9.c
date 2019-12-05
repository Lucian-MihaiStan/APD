#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_NODES       (10)
#define INITIATOR       (0)

#define SENDER          (0)
#define RECEIVER        (9)

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

void send0To9(int rank)
{
    int i, num, numNeighbours, close = -1, sendFurther = 1;
    int* nextNodes  = firstStepBFS(rank);
    int* neighbours = getNeighbours(rank, &numNeighbours);

    if (rank != SENDER)
    {
        MPI_Recv(
            &num,
            1,
            MPI_INT,
            MPI_ANY_SOURCE,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        if (num == close)
        {
            sendFurther = 0;
        }

        if (rank == RECEIVER)
        {
            printf("Receiver got: %d\n", num);
            sendFurther = 0;
        }

    } else
    {
        num = rand() % 1000;

        printf("Sender sent: %d\n", num);
    }

    if (sendFurther)
    {
        MPI_Send(&num, 1, MPI_INT, nextNodes[9], MPI_TAG_UB, MPI_COMM_WORLD);
    }

    for (i = 0; i != numNeighbours; ++i)
    {
        if (neighbours[i] != nextNodes[9])
        {
            MPI_Send(
                &close,
                1,
                MPI_INT,
                neighbours[i],
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );  
        }
    }

    free(nextNodes);
    free(neighbours);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    int rank;

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    send0To9(rank);

    MPI_Finalize();

    return 0;
}
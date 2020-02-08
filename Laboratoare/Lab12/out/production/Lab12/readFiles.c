#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/**
 * @author cristian.chilipirea
 *
 * Create files using
 * dd if=/dev/urandom of=1.txt bs=1024 count=104800
 * dd if=/dev/urandom of=2.txt bs=1024 count=104800
 */
#define MIN(x, y)       (((x) < (y)) ? (x) : (y))

FILE** files;
int NFiles;
int NReps;
int N;
int P;
char** buff;
int* threadIds;
pthread_t* threads;

void getArgs(int argc, char** argv)
{
    if (argc < 5)
    {
        printf("Not enough paramters: ./program N NReps P NFiles\n");
        exit(1);
    }

    N       = atoi(argv[1]);
    NReps   = atoi(argv[2]);
    P       = atoi(argv[3]);
    NFiles  = atoi(argv[4]);
}

void init()
{
    int i;

    buff = malloc(P * sizeof(*buff));
    if (buff == NULL)
    {
        printf("Malloc failed for the buffer.");
        exit(1);
    }

    for (i = 0; i != P; ++i)
    {
        buff[i] = malloc(N * sizeof(*buff[i]))
    }

    threads = malloc(P * sizeof(*threads));
    if (threads == NULL)
    {
        printf("Malloc failed for threads.\n");
        finalise();
        exit(1);
    }

    threadIds = malloc(P * sizeof(*threadIds));
    if (threadIds == NULL)
    {
        printf("Malloc failed for thread ids.\n");
        finalise();
        exit(1);
    }

    files = malloc(NFiles * sizeof(*files));
    if (files == NULL)
    {
        printf("Malloc failed for files.\n");
        finalise();
        exit(1);
    }

    for (i = 0; i != NFiles; ++i)
    {
        files[i] = fopen(#(i).)
    }

    file = fopen(fileName, "rb");
    if (file == NULL)
    {
        printf("Failed to open input file.\n");
        finalise();
        exit(1);
    }
}

void finalise()
{
    if (v != NULL)
    {
        free(v);
    }

    if (threads != NULL)
    {
        free(threads);
    }

    if (threadIds != NULL)
    {
        free(threadIds);
    }

    if (fileName != NULL)
    {
        free(fileName);
    }

    if (file != NULL)
    {
        fclose(file);
    }
}

void* parReadFiles(void* arg)
{
    int i, j;

    int tid     = *(int*)arg;

    int start   = tid * ceil((double)N / P);
    int end     = MIN(N, (tid + 1) * ceil((double)N / P));

    for (i = 0; i != NReps; ++i)
    {
        fread(v + start, 0xff, (end - start) * sizeof(*v));
    }
}

int main(int argc, char** argv)
{
    getArgs(argc, argv);
    init();

    for (i = 0; i != P; ++i)
    {
        threadIds[i] = i;

        pthread_create(threads + i, NULL, parMemset, threadIds + i);
    }

    for (i = 0; i != P; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    finalise();
    
    return 0;
}
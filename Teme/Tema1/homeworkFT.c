#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <complex.h>


#define ASSERT(condition, msg, retVal)          \
    do                                          \
    {                                           \
        if (condition)                          \
        {                                       \
            fprintf(stderr, msg);               \
            return retVal;                      \
        }                                       \
    } while (0)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define PI (3.1415926535897)

typedef enum
{
    STATUS_OK,
    STATUS_INCORRECT_ARGS,
    STATUS_ALLOCATION_FAIL,
    STATUS_PTHREAD_FAIL,
    STATUS_INPUT_ERROR,
    STATUS_OUTPUT_ERROR
} STATUS;

typedef struct
{
    int threadID, N, numThreads;
    double* x;
    double complex* X;
} args_t;


STATUS getArgs(
    int argc,
    char** argv,
    char** inputFile,
    char** outputFile,
    int* numThreads)
{
    ASSERT(
        argc < 4,
        "Usage: ./homeworkFT inputFile.txt outputFile.txt numThreads\n",
        STATUS_INCORRECT_ARGS
    );

    *inputFile = strdup(argv[1]);
    ASSERT(
        *inputFile == NULL,
        "Unable to allocate inputFile.\n",
        STATUS_ALLOCATION_FAIL
    );

    *outputFile = strdup(argv[2]);
    ASSERT(
        *outputFile == NULL,
        "Unable to allocate outputFile.\n",
        STATUS_ALLOCATION_FAIL
    );

    *numThreads = atoi(argv[3]);
    ASSERT(
        *numThreads == 0,
        "Incorrect numThreads.\n",
        STATUS_INCORRECT_ARGS
    );

    return STATUS_OK;
}

STATUS getInput(char* inputFile, double** x, int* N)
{
    int retVal  = 0;
    FILE *input = fopen(inputFile, "rt");
    ASSERT(
        input == NULL,
        "Unable to open inputFile.\n",
        STATUS_INPUT_ERROR
    );

    retVal = fscanf(input, "%d", N);
    ASSERT(retVal != 1, "Unable to read from inputFile.\n", STATUS_INPUT_ERROR);

    *x = malloc(*N * sizeof(double));
    ASSERT(
        *x == NULL,
        "Unable to allocate memory for input.\n",
        STATUS_ALLOCATION_FAIL
    );

    for (int i = 0; i != *N; ++i)
    {
        retVal = fscanf(input, "%lf", (*x) + i);
        ASSERT(
            retVal != 1,
            "Unable to read from inputFile.\n",
            STATUS_INPUT_ERROR
        );
    }

    fclose(input);

    return STATUS_OK;
}

STATUS allocVectors(
    double complex** X,
    pthread_t** tids,
    args_t** args,
    int N,
    int numThreads)
{
    *X = malloc(N * sizeof(double complex));
    ASSERT(
        *X == NULL,
        "Unable to allocate memory for result.\n",
        STATUS_ALLOCATION_FAIL
    );

    *tids = malloc(numThreads * sizeof(pthread_t));
    ASSERT(
        *tids == NULL,
        "Unable to allocate memory for tids.\n",
        STATUS_ALLOCATION_FAIL
    );

    *args = malloc(numThreads * sizeof(args_t));
    ASSERT(
        *args == NULL,
        "Unable to allocate memory for thread args.\n",
        STATUS_ALLOCATION_FAIL
    );

    return STATUS_OK;
}

STATUS printOutput(char* outputFile, double complex* X, int N)
{
    FILE* output = fopen(outputFile, "wt");
    ASSERT(
        output == NULL,
        "Unable to open outputFile.\n",
        STATUS_OUTPUT_ERROR
    );

    fprintf(output, "%d\n", N);

    for (int i = 0; i != N; ++i)
    {
        fprintf(output, "%lf %lf\n", creal(X[i]), cimag(X[i]));
    }

    fclose(output);

    return STATUS_OK;
}

void* fourierTransform(void* arg)
{
    args_t* args = (args_t*)arg;

    int threadID        = args->threadID;
    int N               = args->N;
    int numThreads      = args->numThreads;
    double* x           = args->x;
    double complex* X   = args->X;

    int start   = threadID * ceil((double)N / numThreads);
    int end     = MIN(N, (threadID + 1) * ceil((double)N / numThreads));
    double complex tmp;

    for (int i = start; i != end; ++i)
    {
        tmp = 0.0 + 0.0 * I;

        for (int j = 0; j != N; ++j)
        {
            tmp += x[j] * cexp(-I * 2.0 * PI * i * j / N);
        }

        X[i] = tmp;
    }

    return NULL;
}

int main(int argc, char** argv)
{
    STATUS retVal       = STATUS_OK;
    char* inputFile     = NULL;
    char* outputFile    = NULL;
    int numThreads      = 1;

    retVal = getArgs(argc, argv, &inputFile, &outputFile, &numThreads);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    int N               = 0;
    double* x           = NULL;
    double complex* X   = NULL;
    pthread_t* tids     = NULL;
    args_t* args        = NULL;

    retVal = getInput(inputFile, &x, &N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    retVal=allocVectors(&X, &tids, &args, N, numThreads);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    for (int i = 0; i != numThreads; ++i)
    {
        args[i].threadID    = i;
        args[i].N           = N;
        args[i].numThreads  = numThreads;
        args[i].x           = x;
        args[i].X           = X;

        pthread_create(tids + i, NULL, fourierTransform, args + i);
    }

    for (int i = 0; i != numThreads; ++i)
    {
        pthread_join(tids[i], NULL);
    }

    retVal = printOutput(outputFile, X, N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    free(inputFile);
    free(outputFile);
    free(x);
    free(X);
    free(tids);
    free(args);
    
    return 0;
}

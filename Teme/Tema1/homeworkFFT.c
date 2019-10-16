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
    double complex* x;
    double complex* X;
    pthread_barrier_t* barrier;
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

STATUS getInput(char* inputFile, double complex** x, int* N)
{
    int retVal  = 0;
    float inputVal;

    FILE *input = fopen(inputFile, "rt");
    ASSERT(
        input == NULL,
        "Unable to open inputFile.\n",
        STATUS_INPUT_ERROR
    );

    retVal = fscanf(input, "%d", N);
    ASSERT(retVal != 1, "Unable to read from inputFile.\n", STATUS_INPUT_ERROR);

    *x = malloc(*N * sizeof(double complex));
    ASSERT(
        *x == NULL,
        "Unable to allocate memory for input.\n",
        STATUS_ALLOCATION_FAIL
    );

    for (int i = 0; i != *N; ++i)
    {
        retVal = fscanf(input, "%f", &inputVal);
        ASSERT(
            retVal != 1,
            "Unable to read from inputFile.\n",
            STATUS_INPUT_ERROR
        );

        (*x)[i] = inputVal + 0.0 * I;
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

STATUS printOutput(char* outputFile, double complex* x, int N)
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
        fprintf(output, "%.3lf %.3lf\n", creal(x[i]), cimag(x[i]));
    }

    fclose(output);

    return STATUS_OK;
}

int bitReverse(int x, int log2N) 
{ 
    int n = 0; 

    for (int i = 0; i < log2N; ++i) 
    { 
        n <<= 1; 
        n |= x & 1; 
        x >>= 1; 
    }

    return n; 
}

void* fastFourierTransform(void* arg)
{
    args_t* args = (args_t*)arg;

    int threadID                = args->threadID;
    int N                       = args->N;
    int numThreads              = args->numThreads;
    double complex* x           = args->x;
    double complex* X           = args->X;
    pthread_barrier_t* barrier  = args->barrier;

    int start   = threadID * ceil((double)N / numThreads);
    int end     = MIN(N, (threadID + 1) * ceil((double)N / numThreads));

    int cN, log2N, work = 1;

    for (log2N = 0, cN = N; cN > 1; cN /= 2, ++log2N);
 
    for (int i = start; i < end; ++i)
    {
        X[i] = x[bitReverse(i, log2N)];
    }

    for (int width = 2; width <= N; width *= 2)
    {
        int mid = width / 2;
        double complex w = 1.0 + 0.0 * I;
        double complex wm = cexp(-I * M_PI / mid);

        if (end - start == mid)
        {
            if ((end / mid) & 1)
            {
                end -= mid;
                work = 0;
            } else
            {
                start -= mid;
            }
        }

        if (work)
        {
            for (int i = 0; i < mid; ++i, w *= wm)
            {
                for (int j = start + i; j < end; j += width)
                {
                    double complex tmp = w * X[j + mid];

                    X[j + mid] = X[j] - tmp;
                    X[j] += tmp;
                }
            }
        }

        pthread_barrier_wait(barrier);
    }

    // muie PGP

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
    double complex* x   = NULL;
    double complex* X   = NULL;
    pthread_t* tids     = NULL;
    args_t* args        = NULL;

    pthread_barrier_t barrier;

    retVal = getInput(inputFile, &x, &N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    retVal=allocVectors(&X, &tids, &args, N, numThreads);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    pthread_barrier_init(&barrier, NULL, numThreads);

    for (int i = 0; i != numThreads; ++i)
    {
        args[i].threadID    = i;
        args[i].N           = N;
        args[i].numThreads  = numThreads;
        args[i].x           = x;
        args[i].X           = X;
        args[i].barrier     = &barrier;

        pthread_create(tids + i, NULL, fastFourierTransform, args + i);
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
    pthread_barrier_destroy(&barrier);
    
    return 0;
}

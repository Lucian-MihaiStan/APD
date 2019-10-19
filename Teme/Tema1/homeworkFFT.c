#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <complex.h>


/**
    Verifica o conditie si daca aceasta este indeplinita, se returneaza valoarea
    specificata.
*/
#define ASSERT(condition, msg, retVal)          \
    do                                          \
    {                                           \
        if (condition)                          \
        {                                       \
            fprintf(stderr, msg);               \
            return retVal;                      \
        }                                       \
    } while (0)

/* In loc de fmin */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
    Colectie de valori ce vor fi returnate de functiile programului pentru
    diferite scenarii de apelare.
*/
typedef enum
{
    STATUS_OK,
    STATUS_INCORRECT_ARGS,
    STATUS_ALLOCATION_FAIL,
    STATUS_PTHREAD_FAIL,
    STATUS_INPUT_ERROR,
    STATUS_OUTPUT_ERROR
} STATUS;

/**
    Parametrii ce vor fi pasati unui thread.
*/
typedef struct
{
    int step, N, numThreads;
    double complex* output;
    double complex* buffer;
} THREAD_ARGS;


/**
    Parseaza argumentele primite de program si le salveaza in variabile.
*/
STATUS getArgs(
    int argc,
    char** argv,
    char** inputFile,
    char** outputFile,
    int* numThreads)
{
    ASSERT(
        argc < 4,
        "Usage: ./homeworkFFT inputFile.txt outputFile.txt numThreads\n",
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
        *numThreads <= 0,
        "Incorrect numThreads.\n",
        STATUS_INCORRECT_ARGS
    );

    return STATUS_OK;
}

/**
    Citeste datele de intrare din fisierul de input.
*/
STATUS getInput(char* inputFile, double complex** x, int* N)
{
    int retVal      = 0;
    int n           = 0;
    double inputVal = 0.0;

    FILE *input = fopen(inputFile, "rt");
    ASSERT(
        input == NULL,
        "Unable to open inputFile.\n",
        STATUS_INPUT_ERROR
    );

    retVal = fscanf(input, "%d", &n);
    ASSERT(retVal != 1, "Unable to read from inputFile.\n", STATUS_INPUT_ERROR);
    *N = n;

    *x = malloc(n * sizeof(double complex));
    ASSERT(
        *x == NULL,
        "Unable to allocate memory for input.\n",
        STATUS_ALLOCATION_FAIL
    );

    for (int i = 0; i != n; ++i)
    {
        retVal = fscanf(input, "%lf", &inputVal);
        ASSERT(
            retVal != 1,
            "Unable to read from inputFile.\n",
            STATUS_INPUT_ERROR
        );

        // se formeaza numarul citit, sub forma de numar complex
        (*x)[i] = inputVal + 0.0 * I;
    }

    fclose(input);

    return STATUS_OK;
}

/**
    Aloca vectorul auxiliar folosit de FFT.
*/
STATUS allocVector(double complex** X, int N)
{
    *X = malloc(N * sizeof(double complex));
    ASSERT(
        *X == NULL,
        "Unable to allocate memory for result.\n",
        STATUS_ALLOCATION_FAIL
    );

    return STATUS_OK;
}

/**
    Afiseaza valorile calculate de FFT.
*/
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
        fprintf(output, "%.3lf %.3lf\n", creal(X[i]), cimag(X[i]));
    }

    fclose(output);

    return STATUS_OK;
}

/**
    Dealoca toata memoria alocata.
*/
void freeMemory(
    char* inputFile,
    char* outputFile,
    double complex* x,
    double complex* X)
{
    free(inputFile);
    free(outputFile);
    free(x);
    free(X);
}

/**
    Calculeaza valorile efective ale outputului la un anumit pas.
*/
inline void calculateValues(
    double complex* buffer,
    double complex* output,
    int N,
    int step)
{
    for (int i = 0; i < N; i += step << 1)
    {
        double complex tmp      = cexp(-I * M_PI * i / N) * output[i + step];
        buffer[i >> 1]          = output[i] + tmp;
        buffer[(i + N) >> 1]    = output[i] - tmp;
    }
}

/**
    FFT recursiv si paralel.
*/
void* recursiveFFT(void* arg)
{   
    THREAD_ARGS* args       = (THREAD_ARGS*)arg;

    double complex* output  = args->output;
    double complex* buffer  = args->buffer;
    int step                = args->step;
    int N                   = args->N;
    int numThreads          = args->numThreads;

    if (step < N)
    {
        pthread_t tid;
        THREAD_ARGS nextArgs[2];

        // Se creeaza parametrii urmatoarelor apeluri recursive
        for (int i = 0; i < 2; ++i)
        {
            nextArgs[i].step       = step << 1;
            nextArgs[i].N          = N;
            nextArgs[i].numThreads = numThreads;
        }

        nextArgs[0].output = buffer;
        nextArgs[0].buffer = output;
        nextArgs[1].output = buffer + step;
        nextArgs[1].buffer = output + step;

        if ((step == 1 && numThreads > 1)
            || (step == 2 && numThreads == 4))
        {
            // Se va crea un thread pentru unul dintre apelurile recursive, iar
            // celalalt apel se executa pe threadul curent (secvential)
            pthread_create(&tid, NULL, recursiveFFT, &nextArgs);
            recursiveFFT(nextArgs + 1);

            pthread_join(tid, NULL);
        } else
        {
            // Ambele apeluri sunt secventiale
            recursiveFFT(nextArgs);
            recursiveFFT(nextArgs + 1);
        }
 
        calculateValues(buffer, output, N, step);
    }

    return NULL;
}

/**
    Initiaza algoritmul FFT.
*/
void fastFourierTransform(double complex* x, double complex* X, int N, int numThreads)
{
    memcpy(X, x, N * sizeof(double complex));

    THREAD_ARGS nextArgs;

    nextArgs.output     = X;
    nextArgs.buffer     = x;
    nextArgs.step       = 1;
    nextArgs.N          = N;
    nextArgs.numThreads = numThreads;
 
    recursiveFFT(&nextArgs);
}

int main(int argc, char** argv)
{
    STATUS retVal       = STATUS_OK;
    char* inputFile     = NULL;
    char* outputFile    = NULL;
    int numThreads      = 0;

    retVal = getArgs(argc, argv, &inputFile, &outputFile, &numThreads);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    double complex* x   = NULL;
    double complex* X   = NULL;
    int N               = 0;

    retVal = getInput(inputFile, &x, &N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    retVal = allocVector(&X, N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    fastFourierTransform(x, X, N, numThreads);

    retVal = printOutput(outputFile, x, N);
    ASSERT(retVal != STATUS_OK, " ", retVal);

    freeMemory(inputFile, outputFile, x, X);
    
    return 0;
}

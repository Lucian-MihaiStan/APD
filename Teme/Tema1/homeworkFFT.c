#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <complex.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

char* inputFile;
char* outputFile;
int N, numThreads, threadCreated;
double complex* x;
double complex* X;
pthread_barrier_t barrier;
pthread_mutex_t mutex;

typedef struct
{
    double complex* output;
    double complex* buffer;
    int step;
} args_t;

void getArgs(
    int argc,
    char** argv)
{
    if (argc < 4)
    {
        exit(-1);
    };

    inputFile = strdup(argv[1]);
    outputFile = strdup(argv[2]);
    numThreads = atoi(argv[3]);
}

void getInput(void)
{
    float inputVal;

    FILE *input = fopen(inputFile, "rt");

    fscanf(input, "%d", &N);

    x = malloc(N * sizeof(double complex));

    for (int i = 0; i != N; ++i)
    {
        fscanf(input, "%f", &inputVal);
        x[i] = inputVal + 0.0 * I;
    }

    fclose(input);
}

void allocVectors(void)
{
    X = malloc(N * sizeof(double complex));
}

void printOutput(void)
{
    FILE* output = fopen(outputFile, "wt");

    fprintf(output, "%d\n", N);

    for (int i = 0; i != N; ++i)
    {
        fprintf(output, "%.3lf %.3lf\n", creal(x[i]), cimag(x[i]));
    }

    fclose(output);
}

void* __fft(void*);

void _fft(double complex* buffer, double complex* output, int step)
{
    if (step < N)
    {
        pthread_t tid;
        args_t nextArgs;

        if (step == 2 && numThreads == 4)
        {
            nextArgs.output  = buffer;
            nextArgs.buffer  = output;
            nextArgs.step    = step * 2;

            pthread_create(&tid, NULL, __fft, &nextArgs);
            _fft(output + step, buffer + step, step * 2);

            pthread_join(tid, NULL);
        } else
        {
            _fft(output, buffer, step * 2);
            _fft(output + step, buffer + step, step * 2);
        }        
 
        for (int i = 0; i < N; i += 2 * step)
        {
            double complex tmp = cexp(-I * M_PI * i / N) * output[i + step];
            buffer[i / 2]      = output[i] + tmp;
            buffer[(i + N)/2]  = output[i] - tmp;
        }
    }
}

void* __fft(void* arg)
{   
    args_t* args = (args_t*)arg;

    double complex* output  = args->output;
    double complex* buffer  = args->buffer;
    int step                = args->step;

    if (step < N)
    {
        pthread_t tid;
        args_t nextArgs;

        if ((step == 1 && numThreads > 1) || (step == 2 && numThreads == 4))
        {
            nextArgs.output  = buffer;
            nextArgs.buffer  = output;
            nextArgs.step    = step * 2;

            pthread_create(&tid, NULL, __fft, &nextArgs);
            _fft(output + step, buffer + step, step * 2);

            pthread_join(tid, NULL);
        } else
        {
            _fft(output, buffer, step * 2);
            _fft(output + step, buffer + step, step * 2);
        }
 
        for (int i = 0; i < N; i += 2 * step)
        {
            double complex tmp = cexp(-I * M_PI * i / N) * output[i + step];
            buffer[i / 2]      = output[i] + tmp;
            buffer[(i + N)/2]  = output[i] - tmp;
        }
    }

    return NULL;
}
 
void fft()
{
    memcpy(X, x, N * sizeof(double complex));

    args_t nextArgs;

    nextArgs.output  = X;
    nextArgs.buffer  = x;
    nextArgs.step    = 1;
 
    __fft(&nextArgs);
}

int main(int argc, char** argv)
{
    getArgs(argc, argv);
    getInput();
    allocVectors();

    pthread_barrier_init(&barrier, NULL, numThreads);
    pthread_mutex_init(&mutex, NULL);

    fft();

    printOutput();

    free(inputFile);
    free(outputFile);
    free(x);
    free(X);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

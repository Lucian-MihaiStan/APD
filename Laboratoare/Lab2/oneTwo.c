#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define NUM_PRINTS		(1000)

void* one(void* arg)
{
	int i;
	pthread_barrier_t* barrier = (pthread_barrier_t*)arg;

	for (i = 0; i < NUM_PRINTS; ++i)
	{
		printf("1 ");
	}
	printf("\n");

	pthread_barrier_wait(barrier);
}

void* two(void* arg)
{
	int i;
	pthread_barrier_t* barrier = (pthread_barrier_t*)arg;

	pthread_barrier_wait(barrier);

	for (i = 0; i < NUM_PRINTS; ++i)
	{
		printf("2 ");
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int i, P = 2;
	pthread_t tid[P];
	pthread_barrier_t barrier;

	pthread_barrier_init(&barrier, NULL, 2);

	pthread_create(&tid[0], NULL, one, &barrier);
	pthread_create(&tid[1], NULL, two, &barrier);

	for(i = 0; i < 2; i++)
	{
		pthread_join(tid[i], NULL);
	}

	return 0;
}
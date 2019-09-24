#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

void* threadFunction(void *var)
{
	int thread_id = *(int*)var;

	for (int i = 1; i; --i)
	{
		printf("Hello world from thread %i\n", thread_id);
	}
}

void* secondThread(void *var)
{
	int thread_id = *(int*)var;

	for (int i = 1; i; --i)
	{
		printf("Second function on thread %i\n", thread_id);
	}
}

int main(int argc, char **argv)
{
	int P = sysconf(_SC_NPROCESSORS_ONLN);
	int i;

	printf("Number of cores: %d\n", P);

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	// for(i = 0; i < P; i++) {
	// 	pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	// }
	pthread_create(&(tid[0]), NULL, threadFunction, &(thread_id[0]));
	pthread_create(&(tid[1]), NULL, secondThread, &(thread_id[1]));

	for(i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}

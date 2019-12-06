#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

/**
 * @author cristian.chilipirea
 * 
 */

int n = 1024*1024*100;

int main(int argc, char * argv[]) {
	int a;
	int i, j;
	clock_t begin = clock();

	{
		int c;
		for(i=0;i<n;i++)
			c = sqrt(a);
	}


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent);
	return 0;
}
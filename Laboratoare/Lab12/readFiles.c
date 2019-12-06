#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

/**
 * @author cristian.chilipirea
 * Create files using
 * dd if=/dev/urandom of=1.txt bs=1024 count=102400
 * dd if=/dev/urandom of=2.txt bs=1024 count=102400
 */
 
int n = 1024*1024*100;
char buf1[104857600];
char buf2[104857600];
int main(int argc, char * argv[]) {

	FILE * f = fopen("1.txt","rb");
	FILE * g = fopen("2.txt","rb");
	clock_t begin = clock();

	fread(buf1,sizeof(char),n,f);
	fread(buf2,sizeof(char),n,g);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent);
	fclose(f);
	fclose(g);
	return 0;
}
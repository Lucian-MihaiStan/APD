#include <stdio.h>

unsigned char c[10000];

int main(void)
{
	FILE* in = fopen("input_files/lena.pgm", "r");
	char str[3] = {0};
	int width, height, maxval;

	fread(str, 1, 2, in);
	fscanf(in, "%d %d\n", &width, &height);
	fscanf(in, "%d\n", &maxval);

	fread(c, 1, 10000, in);

	// for (int i = 0; i < 10000; ++i)
	// {
	// 	printf("%c ", c[i]);
	// }

	printf("%s %d %d %d\n", str, width, height, maxval);

	printf("\n\n");

	for (int i = 0; i < 10000; ++i)
	{
		printf("%d ", c[i]);
	}
}
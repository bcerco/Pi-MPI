#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int 
*buildMatrix(int rows, int cols, int fill) 
{

	int *matrix = (int *)malloc(rows * cols * sizeof(int));

	if (fill) {
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				*(matrix + row*cols + col) = rand() % 51;
			}
		}
	}

	return matrix;
}

int 
main(int argc, char *argv[]) 
{

	srand(time(NULL));

	if (argc != 4) {
		printf("Number of args: %i\n", argc);
		printf("Expecting 4 arguments");
		exit(EXIT_FAILURE);	
	}

	int rows = atoi(argv[1]);
	int overlaps = atoi(argv[2]);
	int cols = atoi(argv[3]);

	int *matrixA = buildMatrix(rows, overlaps, 1);
	int *matrixB = buildMatrix(overlaps, cols, 1);
	int *matrixC = buildMatrix(rows, cols, 0);	

	int sum;

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			sum = 0;
			for (int overlap = 0; overlap < overlaps; ++overlap) {
				sum += *(matrixA + row*overlaps + overlap) *
					*(matrixB + col + overlap*cols);
			}
			*(matrixC + row*cols + col) = sum;
		}
	}

	free(matrixA);
	free(matrixB);
	free(matrixC);
	printf("Done\n");

}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *buildMatrix(int rows, int cols, int fill) {

	int *matrix = (int *)malloc(rows * cols * sizeof(int));
	int num;
	if (fill) {
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				int num = rand() % 10;
				*(matrix + row*cols + col) = num;
				printf("%i ", num);
			}
			printf("\n");
		}
	}

	printf("\n");
	return matrix;
}

int main(int argc, char *argv[]) {

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

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < overlaps; ++col) {
			printf("%i ", *(matrixA +  row*overlaps + col));
		}
		printf("\n");
	}

	printf("\n");

	for (int row = 0; row < overlaps; ++row) {
		for (int col = 0; col < cols; ++col) {
			printf("%i ", *(matrixB +  row*cols + col));
		}
		printf("\n");
	} 

	int sum;

	printf("\n");

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			sum = 0;
			for (int overlap = 0; overlap < overlaps; ++overlap) {
				//printf("\n%i * %i", *(matrixA + row*overlaps + overlap), *(matrixB + col + overlap*cols));
				sum += *(matrixA + row*overlaps + overlap) * *(matrixB + col + overlap*cols);
			}
			//printf("\n\n");
			*(matrixC + row*cols + col) = sum;
		}
	}
	
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			printf("%i ", *(matrixC +  row*cols + col));
		}
		printf("\n");
	}

	printf("\n");

	printf("Done");

}
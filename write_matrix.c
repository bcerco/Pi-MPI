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

void
writeMatrix(int rows, int cols, int *matrix, char * filename) {

	FILE *outFile;

	outFile = fopen(filename,  "w");

	for (int row = 0; row < rows; ++row) {
		int col = 0;
		if (col < cols) {
			fprintf(outFile, "%i", *(matrix + row*cols + col));
		}
		for (col = 1; col < cols; ++col) {
			fprintf(outFile, ",%i", *(matrix + row*cols + col));
		}
		fprintf(outFile, "\n");
	}

	fclose(outFile);
}

void
writeParams(int firstParam, int secondParam, int thirdParam, char * filename) {

	FILE *outFile;

	outFile = fopen(filename, "w");

	fprintf(outFile, "%i,%i,%i", firstParam, secondParam, thirdParam);

	fclose(outFile);
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

	writeMatrix(rows, overlaps, matrixA, "matrixA.txt");
	writeMatrix(overlaps, cols, matrixB, "matrixB.txt");
	writeParams(rows, overlaps, cols, "sizes.txt");

	free(matrixA);
	free(matrixB);

	printf("Done\n");

	return(EXIT_SUCCESS);

}

#include <stdio.h>
#include <stdlib.h>

struct arr *wMatrix = NULL;
struct arr *yMatrix = NULL;
struct arr *trainMatrix = NULL;
int kSize;

struct arr{
    int rows;
    int cols;
    double **matrix ; 
};

typedef struct arr arr;

arr* create_arr(int rows, int cols, int train)
{
	arr *rMatrix;
	rMatrix = malloc(sizeof *rMatrix);
	rMatrix->rows = rows;
	if(train == 1) rMatrix->cols = cols+1;	// If we are using train data, we need a 1's column
	if(train == 0) rMatrix->cols = cols;

	rMatrix->matrix = (double**) calloc((rows),sizeof(double*));
	int i;
	for(i = 0; i < rows; i++)
	{
		rMatrix->matrix[i] = (double*)calloc(cols,sizeof(double));
		if(train) rMatrix->matrix[i][0] = 1.0;
	}
	return rMatrix;
}

#ifndef LEARN_H_
#define LEARN_H_

void FillMatrix(int argc, char** argv);

arr* InvertMatrix(arr* matrix);		// Calls GaussEliminate()

arr* TransposeMatrix(arr* matrix);

arr* GaussEliminate(arr* matrix);	// Calls SubtractRows()

arr* GetWeights();

void GetPrintPrices(int argc, char** argv);

void FreeMatrix(arr* matrix);

void AssignMatrix();

void PrintMatrix(arr* matrix);

arr* MultiplyMatrix(arr* matrixA, arr* matrixB);

arr* SubtractRows(arr* tempMatrix, arr* idMatrix);	// Called by GaussEliminate()


void Printy();
void Printw();

#endif


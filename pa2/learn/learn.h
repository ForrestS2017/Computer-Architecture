#include <stdio.h>
#include <stdlib.h>

double *wMatrix = NULL;
double *yMatrix = NULL;
struct arr *trainMatrix = NULL;
struct arr *testMatrix = NULL;
int kSize, nSize, mSize;

struct arr{
    int rows;
    int cols;
    double **matrix ; // change the member!
};

typedef struct arr arr;

arr* create_arr(int rows, int cols, int train)
{
	arr *rMatrix = malloc(sizeof *rMatrix);
	rMatrix->rows = rows;
	if(train == 1) rMatrix->cols = cols+1;
	if(train == 0) rMatrix->cols = cols;

	rMatrix->matrix = (double**) malloc((rows)*sizeof(double*));
	int i;
	for(i = 0; i < rows; i++)
	{
		rMatrix->matrix[i] = (double*)calloc(cols,sizeof(double));
		if(train) rMatrix->matrix[i][0] = 1;
	}
	return rMatrix;
}

#ifndef LEARN_H_
#define LEARN_H_

void FillMatrix(int argc, char** argv);

arr* InvertMatrix(arr* matrix);

arr* TransposeMatrix(arr* matrix);

arr* GaussEliminate(arr* matrix);

double* GetWeights();

void FreeMemory();

void AssignMatrix();

void PrintMatrix(arr* matrix);

int Check_1(arr* matrix, int row);

arr* MultiplyMatrix(arr* matrixA, arr* matrixB);

void SubtractRows(arr* tempMatrix, arr* idMatrix, int row);


void Printy();
void Printw();
void Printm();

#endif

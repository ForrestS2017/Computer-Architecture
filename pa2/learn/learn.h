#include <stdio.h>
#include <stdlib.h>

double *wMatrix = NULL;
double *yMatrix = NULL;
double **trainMatrix = NULL;
double **testMatrix = NULL;
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
	rMatrix->matrix = (double**) malloc((rows)*sizeof(double*));
	int i;
	switch(train)
	{
	case 1:
		for(i = 0; i <= rows; i++)
		{
			trainMatrix[i] = (double*)calloc(cols,sizeof(double));
			trainMatrix[i][0] = 1;
		}
		break;
	case 0:
		for(i = 0; i < rows; i++)
		{
			trainMatrix[i] = (double*)calloc(cols,sizeof(double));
		}
		break;
	}
	return rMatrix;
}

#ifndef LEARN_H_
#define LEARN_H_

void FillMatrix(int argc, char** argv);

void InvertMatrix(double** matrix);

double** TransposeMatrix(double** matrix);

double** GaussEliminate(double** matrix);

double* GetWeights();

void FreeMemory();

void AssignMatrix();

void PrintMatrix(double** matrix);


void Printy();
void Printw();
void Printm();

#endif

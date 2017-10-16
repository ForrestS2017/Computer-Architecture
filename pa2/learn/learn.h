#include <stdio.h>
#include <stdlib.h>

double *wmatrix = NULL;
double *yMatrix = NULL;
double **trainMatrix = NULL;
int kSize, nSize, mSize;

#ifndef LEARN_H_
#define LEARN_H_

void FillMatrix(int argc, char** argv);

void InvertMatrix(double** matrix);

void TransposeMatrix(double** matrix);

void FreeMemory();

void AssignMatrix();

void PrintMatrix();

void Printy();
void Printw();

#endif

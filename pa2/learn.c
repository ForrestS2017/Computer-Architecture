#include "learn.h"
#include <math.h>

int main(int argc, char** argv)
{
	if( argc < 3)
	{
		printf("error");
		exit(0);
	}
	FillMatrix(argc, argv);
	wMatrix = GetWeights();
	GetPrintPrices(argc, argv);
	return 0;
}

arr* GetWeights()
{
	arr * resMatrix;
	arr * tranMatrix;
	
	tranMatrix = TransposeMatrix(trainMatrix);
	resMatrix = MultiplyMatrix(tranMatrix, trainMatrix);
	resMatrix = InvertMatrix(resMatrix);
	resMatrix = MultiplyMatrix(resMatrix, tranMatrix);
	resMatrix = MultiplyMatrix(resMatrix, yMatrix);	// This is now the w-matrix

	return resMatrix;
}

void GetPrintPrices(int argc, char** argv)
{
	// Read the input file and calculate house price line by line
	FILE *fTest;
	int r, i, mSize;
	double temp;

	fTest = fopen(argv[2], "r");

	if(fTest == NULL){
		printf("error");
		exit(0);
	}


	r = fscanf(fTest, "%d\n", &mSize);		// Get m-attribute size
	if(r != 1){printf("error"); exit(0);}		// Error throw

	//initialize counter and while loop for number of prices to be calculated

	double* testMatrix;
	testMatrix = (double*) calloc(mSize , sizeof(double));m
	while(mSize > 0){
		temp = 0.0;
		for(i = 0; i < kSize; i++){
			fscanf(fTest, "%lf,", &testMatrix[i]);
		}
		for(i = 0; i < kSize; i++){
			temp = temp + testMatrix[i]*wMatrix->matrix[i+1][0];
		}

		temp = temp + wMatrix->matrix[0][0];
		printf("%0.0lf\n", temp);
		mSize = mSize-1;

	}
}

arr* TransposeMatrix(arr* matrix)
{
	int i, j, mRows, mCols;
	mRows = matrix->rows;
	mCols = matrix->cols;
	arr* tnMatrix = create_arr(mCols, mRows, 0);

	for(i = 0; i < mRows; i++)
	{
		for(j = 0; j < mCols; j++)
		{
			tnMatrix->matrix[j][i] = matrix->matrix[i][j];
		}
	}

	return tnMatrix;
}

arr* InvertMatrix(arr* matrix)
{
	arr* inMatrix = GaussEliminate(matrix);

	return inMatrix;
}

arr* GaussEliminate(arr* matrix)
{
	arr* idMatrix;	// Your Identity Matrix
	arr* tempMatrix;	// Temporary duplicate matrix of train data to prevent overwitng original
	int i, j, rows,cols;	// Create local variables
	rows = matrix->rows;	
	cols = matrix->cols;
	idMatrix = create_arr(rows, cols, 0);	// Create ID matrix
	tempMatrix= create_arr(rows, cols, 0);

	// Fill temp duplicated train matrix
	for(i = 0; i < rows; i++)
	{
		for(j = 0; j < cols; j++)
		{
			tempMatrix->matrix[i][j] = matrix->matrix[i][j];
		}
	}

	rows = rows < cols ? rows : cols;	// Get smaller dimension to enter 1's
	for(i = 0; i < rows; i++) idMatrix->matrix[i][i] = 1.0;

	// BEGIN GAUSS ELIMINATION

	idMatrix = SubtractRows(tempMatrix, idMatrix);

	return idMatrix;
}

arr* SubtractRows(arr* tempMatrix, arr* idMatrix)
{

	// This function operates on the tempMatrix (input matrix) and Identity matrix separately
	double** curTMatrix;
	double** curIMatrix;
	curTMatrix = tempMatrix->matrix;
	curIMatrix = idMatrix->matrix;

	int i, j, k, l, rows, cols, tRow;
	double c;
	rows = tempMatrix->rows;
	cols = tempMatrix->cols;

	for(i = 0; i < rows; i++)	// Hold current "column" by row
	{
			if(fabs(curTMatrix[i][i]) > 1.0)	// If row does not have a 1 in pivot point
		{
			c = 1.0/curTMatrix[i][i];	// Get constant for current row
			for(k = 0; k < cols; k++)	// Normalize row
			{
				curTMatrix[i][k] = curTMatrix[i][k] * c;
				curIMatrix[i][k] = curIMatrix[i][k] * c;
			}
		}
		for(j = 0; j < rows; j++)	// Iterate through rows to subtract each i,k value. Iterating top to bottom
		{
			if(i == j) continue;	// Skip our own row

			/*********If we have a 0 at a pivot, find another row to get the 1**********/
			if(fabs(curTMatrix[i][i]) < 10e-7 )
			{
				for(l = i+1; l < rows; l++)	// Search each row below for a 1
				{
					if(fabs(curTMatrix[l][i]) != 0.0)	// If the number in the pivot column isn't 0, store the pivot to use as coefficient
					{
						tRow = l;	// tRow holds the supplemented row
						c = 1.0 / curTMatrix[l][i];	// We divide 1 by the coefficient so we multiply the row back by it to get 1 at pivot
						break;
					}
				}
				for(k = 0; k < cols; k++)	// Subtract for the single row using new pivot at tRow
				{
					curTMatrix[i][k] = curTMatrix[i][k] + c * curTMatrix[tRow][k];	// Add the supplement row back * inverse coeff
					curIMatrix[i][k] = curIMatrix[i][k] + c * curIMatrix[tRow][k]; 
				}
			}
			/********* End non-0 pivot fix **********/

			c = curTMatrix[j][i]; // Coeff here is simpyl the row number since we want to make the indicies below our pivot 0
			{
				curTMatrix[j][k] = curTMatrix[j][k] - c * curTMatrix[i][k];		// Subtract the current row * coeff
				curIMatrix[j][k] = curIMatrix[j][k] - c * curIMatrix[i][k];
			}

		}
	}
	return idMatrix;
}

arr* MultiplyMatrix(arr* matrixA, arr* matrixB)
{
	int place, prim, sec;
	double temp, hold;
	arr* resMatrix = create_arr(matrixA->rows, matrixB->cols, 0);	// Returning result matrix
	
	for(place = 0, temp = 0.0; place < matrixA->rows; place++)		// A-row iterator
	{
		for(prim = 0, temp = 0.0; prim < matrixB->cols; prim++, temp = 0.0)	// B-col iterator
		{
			for(sec = 0; sec < matrixB->rows; sec++)	//A-col & B-row iterator
			{
				hold = matrixA->matrix[place][sec]*matrixB->matrix[sec][prim];
				temp = temp + hold;		// Multiply and add each result to a temp double we will put in our resutling matrix
			}
			resMatrix->matrix[place][prim] = temp;		// Store new value
		}
	}
	return resMatrix;
}

void PrintMatrix(arr* matrix)
{
	int i,j;
	for(i = 0; i < matrix->rows; i++)
	{
		for(j = 0; j < matrix->cols; j++)
		{
			printf("%lf", matrix->matrix[i][j]);
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}

void Printy()
{
	int i;
	printf("Y-MATRIX:\n");
	for(i = 0; i < yMatrix->rows; i++)
	{
		printf("%lf", yMatrix->matrix[i][0]);
		printf("\t");
	}
	printf("\n\n");

}




void Printw()
{
	int i;
	printf("W-MATRIX:\n");
	for(i = 0; i < wMatrix->rows; i++)
	{
		printf("%lf", wMatrix->matrix[i][0]);
		printf("\t");
	}
	printf("\n\n");

}

void FillMatrix(int argc, char** argv)
{
	int i, j, r, nSize;
	double temp;

	FILE* fTrain = NULL;
	char* trainingName = NULL;

	trainingName = argv[1];

	fTrain = fopen(trainingName, "r");
	if(fTrain == NULL){
		printf("error");
		exit(0);
	}


	r = fscanf(fTrain, "%d\n", &kSize);		// Get k-attribute size
	if(r != 1){printf("error\n"); exit(0);}		// Error throw

	r = fscanf(fTrain, "%d", &nSize);		// Get n-data size

	yMatrix = create_arr(nSize, 1, 0);
	wMatrix = create_arr(nSize, kSize+1, 0);
	trainMatrix = create_arr(nSize, kSize, 1);

	// Entering Train values

	for(i = 0; i < nSize; i++)
	{
		for(j = 1; j <= kSize; j++){
			fscanf(fTrain, "%lf,", &temp);
			trainMatrix->matrix[i][j] = temp;
		}
		fscanf(fTrain, "%lf,", &temp);
		yMatrix->matrix[i][0] = temp;
	}


	fclose(fTrain);

}

void FreeMatrix(arr* matrix)
{
	int i;
	for(i = 0; i < matrix->rows; i++)
	{
		free(matrix->matrix[i]);
	}

}


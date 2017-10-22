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
	PrintMatrix(trainMatrix);
	wMatrix = GetWeights();
	Printw();
	FreeMatrix(trainMatrix);
	FreeMatrix(testMatrix);
	return 0;
}

arr* GetWeights()
{
	//double** invMatrix;
	arr * resMatrix;
	arr * tranMatrix;
	printf("--Transposed:\n");
	tranMatrix = TransposeMatrix(trainMatrix);
	PrintMatrix(tranMatrix);
	printf("--Mutiplied:\n");
	resMatrix = MultiplyMatrix(tranMatrix, trainMatrix);
	PrintMatrix(resMatrix);
	printf("--Inverted:\n");
	resMatrix = InvertMatrix(resMatrix);
	PrintMatrix(resMatrix);
	printf("--TxMutiplied:\n");
	resMatrix = MultiplyMatrix(resMatrix, tranMatrix);
	PrintMatrix(resMatrix);
	//PrintMatrix(tranMatrix);
	printf("--YxMutiplied:\n");
	resMatrix = MultiplyMatrix(resMatrix, yMatrix);	// This is now the w-matrix
	PrintMatrix(yMatrix);
	PrintMatrix(resMatrix);
	/*printf("--XxMutiplied:\n");
	resMatrix = MultiplyMatrix(testMatrix, resMatrix);
	PrintMatrix(resMatrix);*/

	return resMatrix;
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

	// Begin
	return inMatrix;
}

arr* GaussEliminate(arr* matrix)
{

	//PrintMatrix(idMatrix);

	arr* idMatrix;	// Your Identity Matrix
	arr* tempMatrix;	// Temporary duplicate matrix of train data to prevent overwitng original
	int i, j, rows,cols;
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

	// First step of getting diagonal ones. Second step later to isolate 1 column
	idMatrix = SubtractRows(tempMatrix, idMatrix);
	//PrintMatrix(idMatrix);

	return idMatrix;
}

arr* SubtractRows(arr* tempMatrix, arr* idMatrix)
{


	// DONT DO ELIMINATION BY ROW, DO IT BY SUBTRACTION!!
	double** curTMatrix;
	double** curIMatrix;
	curTMatrix = tempMatrix->matrix;
	curIMatrix = idMatrix->matrix;

	int i, j, k, l, rows, cols, tRow;
	double c;
	rows = tempMatrix->rows;
	cols = tempMatrix->cols;
	//printf("DIMENSIONS\n");
	//PrintMatrix(tempMatrix);
	//PrintMatrix(idMatrix);
	/*c = 1/curTMatrix[0][0];
	//if(curTMatrix[j][i] == 0) c = 0;
	//printf("[%d,%d] = %lf\tc=%lf\n", j, i, curTMatrix[j][i], c);
	for(k = 0; k < cols; k++)	// Subtract the slow-iterating i'th row from the med-iterating j'th row
	{
		curTMatrix[0][k] *= c;
		curIMatrix[0][k] *= c;
	}*/

	//PrintMatrix(tempMatrix);
	for(i = 0; i < rows; i++)	// Hold current "column" by row
	{
		//PrintMatrix(tempMatrix);
		//PrintMatrix(idMatrix);

		if(fabs(curTMatrix[i][i]) > 1.0)
		{
			//printf("normalizing row\n");
			c = 1/curTMatrix[i][i];
			//printf("c: %lf\n", c);
			for(k = 0; k < cols; k++)	// Normalize row
			{
				curTMatrix[i][k] *= c;
				curIMatrix[i][k] *= c;
			}
			//PrintMatrix(tempMatrix);
			//PrintMatrix(idMatrix);
		}
		//printf("\nFocus [%d, %d] = %f\n", i, i, curTMatrix[i][i]);
		for(j = 0; j < rows; j++)	// Iterate through rows to subtract each i,k value
		{
			if(i == j) continue;
			//curTMatrix[i][i] += +0;

			/*********If we have a 0 at a pivot, find another row to get the 1**********/
			if(fabs(curTMatrix[i][i]) < 10e-7 )
			{
				//printf("-----This row has 0 at pivot. i:%d: %lf-----\n", i, fabs(curTMatrix[i][i]));
				//printf("%lf\n", curTMatrix[i][i]);
				for(l = i+1; l < rows; l++)	// Check each row below for a 1
				{
					if(curTMatrix[l][i] != 0)	// If the number in the pivot column isn't 0, subtract the row
					{
						tRow = l;
						c = 1.0 / curTMatrix[l][i];
						//printf("found @ %d %lf\t c=%lf\n", tRow, curTMatrix[l][i], c);
						break;
					}
				}
				for(k = 0; k < cols; k++)	// Subtract for the single row
				{
					//printf("old: %lf * c=%d\n", curTMatrix[i][k], c);
					curTMatrix[i][k] += c*curTMatrix[tRow][k];
					curIMatrix[i][k] += c*curIMatrix[tRow][k];
					//curTMatrix[i][k] += +0.0;
					//curIMatrix[i][k] += +0.0;
					//printf("new: %lf * c=%d\n", curTMatrix[i][k], c);
				}
				curTMatrix[i][i] = 1.0;
				//PrintMatrix(tempMatrix);
				//printf("-----Fixed 0 pivot row %lf-----\n\n", curTMatrix[i][i]);
			}
			/*******************/

			c = curTMatrix[j][i];
			//printf("c: %lf\t",c);
			//if(curTMatrix[j][i] == 0) c = 0;
			//printf("[%d,%d] = %lf\tc=%lf\n", j, i, curTMatrix[j][i], c);
			for(k = 0; k < cols; k++)	// Subtract the slow-iterating i'th row from the med-iterating j'th row
			{
				curTMatrix[j][k] -= c*curTMatrix[i][k];
				curIMatrix[j][k] -= c*curIMatrix[i][k];
				//curTMatrix[j][k] += +0.0;
				//curIMatrix[j][k] += +0.0;
				if (fabs(curTMatrix[j][k]) < 10e-7) curTMatrix[j][k] = +0;
				if (fabs(curIMatrix[j][k]) < 10e-7) curIMatrix[j][k] = +0;
			}

		}

		//printf("CurTMatrix, i:%d\t\n", i);
		//PrintMatrix(tempMatrix);
	}
	//PrintMatrix(tempMatrix);
	//PrintMatrix(idMatrix);
	//printf("start upper triangular\n");
	//PrintMatrix(idMatrix);
/*	for(i = 1; i < rows; i++)	// Hold current "column" by row
	{
		for(j = i-1; j >= 0; j--)	// Iterate through rows to subtract each i,k value
		{
			c = curTMatrix[j][i];
			for(k = 0; k < cols; k++)	// Subtract the slow-iterating i'th row from the med-iterating j'th row
			{
				curTMatrix[j][k] -= c*curTMatrix[i][k];
				curIMatrix[j][k] -= c*curIMatrix[i][k];
				curTMatrix[j][k] += +0.0;
				curIMatrix[j][k] += +0.0;
			}
		}
		PrintMatrix(tempMatrix);
	}*/
	//PrintMatrix(tempMatrix);
	//PrintMatrix(idMatrix);
	return idMatrix;
}

arr* MultiplyMatrix(arr* matrixA, arr* matrixB)
{
	int place, prim, sec;
	double temp, hold;
	arr* resMatrix = create_arr(matrixA->rows, matrixB->cols, 0);
	if(fabs(matrixA->matrix[0][0] - 1362.597246) < 0.00001 )
	{
		printf("TESTING\n");
		PrintMatrix(resMatrix);
	}
	for(place = 0, temp = 0.0; place < matrixA->rows; place++)
	{
		for(prim = 0, temp = 0.0; prim < matrixB->cols; prim++, temp = 0.0)
		{
			for(sec = 0; sec < matrixB->rows; sec++)
			{
				//printf("p:%d s:%d ", prim, sec);
				hold = (matrixB->matrix[sec][prim]*matrixA->matrix[place][sec]);
				temp = temp + hold;
				if(fabs(matrixA->matrix[0][0] - 1362.597246) < 0.00001 )printf("[%lf]*[%lf]:\t%lf\t%lf\n", matrixA->matrix[place][sec], matrixB->matrix[sec][prim], hold, temp);
			}
			//if(matrixB->cols == 1)printf("[%d][%d]: %lf\n", place, prim, temp);
			//printf("%d\t ", temp);
			resMatrix->matrix[place][prim] = temp;
			if(fabs(matrixA->matrix[0][0] - 1362.597246) < 0.00001 )PrintMatrix(resMatrix);
			//if(matrixB->cols == 1)PrintMatrix(resMatrix);
		}
	}

	return resMatrix;
}

void PrintMatrix(arr* matrix)
{
	int i,j;
	printf("Matrix\n");
	//printf("rows:%d, cols:%d\n", matrix->rows, matrix->cols);
	//printf("%f\n", matrix[9][4]);
	for(i = 0; i < matrix->rows; i++)
	{
		for(j = 0; j < matrix->cols; j++)
		{
			printf("%f", matrix->matrix[i][j]);
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
		printf("%f", yMatrix->matrix[i][0]);
		printf("\t");
	}
	printf("\n\n");

}

void Printm()
{
	int i, j;
	printf("M-MATRIX:\n");
	for(i = 0; i < testMatrix->rows; i++)
	{
		for(j = 0; j < testMatrix->cols; j++)
		{
			printf("%f", testMatrix->matrix[i][j]);
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}

void Printw()
{
	int i;
	printf("W-MATRIX:\n");
	for(i = 0; i < wMatrix->rows; i++)
	{
		printf("%f", wMatrix->matrix[i][0]);
		printf("\t");
	}
	printf("\n\n");

}

void FillMatrix(int argc, char** argv)
{
	int i, j, r, s, kSize, nSize, mSize;
	double temp;

	FILE* fTrain = NULL;
	FILE* fTest = NULL;
	char* trainingName = NULL;
	char* testingName = NULL;

	trainingName = argv[1];
	testingName = argv[2];

	fTrain = fopen(trainingName, "r");
	if(fTrain == NULL){
		printf("error: File does not exist\n");
		exit(0);
	}

	fTest = fopen(testingName, "r");
	if(fTest == NULL){
		printf("error: File does not exist\n");
		exit(0);
	}

	r = fscanf(fTrain, "%d\n", &kSize);		// Get k-attribute size
	printf("kSize: %d\n", kSize);
	if(r != 1){printf("error\n"); exit(0);}		// Error throw

	r = fscanf(fTrain, "%d", &nSize);		// Get n-data size
	printf("nSize: %d\n", nSize);

	s = fscanf(fTest, "%d\n", &mSize);		// Get k-attribute size
	printf("mSize: %d\n", mSize);
	if(s != 1){printf("error\n"); exit(0);}		// Error throw

	//yMatrix = (double*) calloc((nSize),sizeof(double));
	//wMatrix = (double*) calloc((kSize+1),sizeof(double));

	yMatrix = create_arr(nSize, 1, 0);
	wMatrix = create_arr(nSize, kSize+1, 0);
	trainMatrix = create_arr(nSize, kSize, 1);
	testMatrix = create_arr(mSize, kSize, 0);

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

	// Entering Test values

	for(i = 0; i < mSize; i++)
	{
		for(j = 0; j < kSize; j++){
			fscanf(fTest, "%lf,", &temp);
			testMatrix->matrix[i][j] = temp;
		}
	}
	fclose(fTrain);
	fclose(fTest);

}

void FreeMatrix(arr* matrix)
{
	int i;
	//printf("rows:%d, cols:%d\n", matrix->rows, matrix->cols);
	//printf("%f\n", matrix[9][4]);
	for(i = 0; i < matrix->rows; i++)
	{
		free(matrix->matrix[i]);
	}
	//free(matrix->matrix);
	//free(matrix);


	/*int place, prim, sec;
		double temp;
		for(sec = 0; sec < wMatrix->rows; sec++)
		{
			//printf("p:%d s:%d ", prim, sec);
			wMatrix->matrix[sec] = resMatrix[0];
		}

		for(place = 0; place < testMatrix->rows; place++)
		{
			for(prim = 0, temp = 0; prim < resMatrix->cols; prim++, temp = 0)
			{
				for(sec = 0; sec < resMatrix->rows; sec++)
				{
					//printf("p:%d s:%d ", prim, sec);
					if(sec == 0)
					temp += testMatrix->matrix[place][sec] * resMatrix->matrix[sec][prim];
				}
				//printf("\n[%d][%d]: %d", place, prim, temp);
				//printf("%d\t ", temp);
				resMatrix->matrix[place][prim] = temp;
				PrintMatrix(resMatrix);
			}
		}*/
}

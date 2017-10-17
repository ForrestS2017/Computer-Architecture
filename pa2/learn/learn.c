#include "learn.h"


int main(int argc, char** argv)
{

	if( argc < 3)
	{
		printf("error");
		exit(0);
	}
	FillMatrix(argc, argv);
	//wMatrix = GetWeights();
	//PrintMatrix(trainMatrix);
	//Printy();
	//Printw();
	//Printm();
	InvertMatrix(trainMatrix);
	return 0;
}

arr* InvertMatrix(arr* matrix)
{
	arr* inMatrix = GaussEliminate(matrix);

	// Begin
	return inMatrix;
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

arr* GaussEliminate(arr* matrix)
{

	//PrintMatrix(idMatrix);

	arr* idMatrix;	// Your Identity Matrix
	arr* tempMatrix;	// Temporary duplicate matrix of train data to prevent overwitng original
	int i, j, rows,cols, proper;
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
	for(i = 1; i < rows; i++)
	{
		//proper =  Check_1(tempMatrix, i);
		if(Check_1( tempMatrix, i))
		{
			//so, if the line is improper, we do row operations
			//Simply, create a for loop to iterate through the previous rows.
			//Subtract row j, get coefficient and multiply whole row of both matricies
			// repeat/iterate w/ next row up until i
		}
	}


	return tempMatrix;
}

int Check_1(arr* matrix, row)
{
	//if(matrix->rows > matrix-> cols && row > matrix->cols) return 1;
	int i;
	for(i = 0; i < row; i++)
	{
		if(i < row && matrix->matrix[i][i])
		{
			return 0;
		}
	}
	if(matrix->matrix[row][row] != 1) return 0;

	return 1;
}

double* GetWeights()
{
	//double** invMatrix;
	return NULL;
}



/*void FreeMemory()
{
	int i;
	for(i = 0; i < matrixSize; i++)
	    {
	        free(matrix[i]);
	        free(resultMatrix[i]);
	        //free(tempMatrix[i]);
	    }
	    free(matrix);
	    free(resultMatrix);
	   // free(tempMatrix);
}
*/

void PrintMatrix(arr* matrix)
{
	int i,j;
	printf("Matrix\n");
	printf("rows:%d, cols:%d\n", matrix->rows, matrix->cols);
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
	for(i = 0; i < nSize; i++)
	{
		printf("%f", yMatrix[i]);
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
	for(i = 0; i < (kSize+1); i++)
	{
		printf("%f", wMatrix[i]);
		printf("\t");
	}
	printf("\n\n");

}

void FillMatrix(int argc, char** argv)
{
	int i, j, r, s;
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

	yMatrix = (double*) calloc((nSize),sizeof(double));
	wMatrix = (double*) calloc((kSize+1),sizeof(double));

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
		yMatrix[i]= temp;
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

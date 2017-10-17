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
	PrintMatrix(trainMatrix);
	Printy();
	Printw();
	Printm();
	GaussEliminate(trainMatrix);
	return 0;
}

arr* InvertMatrix(arr* matrix)
{
	arr* idMatrix;
	idMatrix = create_arr(nSize, kSize+1, 0);	// Create ID matrix
	int i, j, rows,cols;
	rows = idMatrix->rows;
	cols = idMatrix->cols;

	rows = rows < cols ? rows : cols;

	for(i = 0; i < rows; i++)
	{
		idMatrix->matrix[i][i] = 1.0;
	}
	printf("NORMAL\n");
	PrintMatrix(matrix);
	// Create temp train matrix
	arr* tempMatrix;
	tempMatrix= create_arr(matrix->rows, matrix->cols-1, 1);
	for(i = 0; i < tempMatrix->rows; i++)
	{
		for(j = 0; j < tempMatrix->cols; j++)
		{
			tempMatrix->matrix[i][j] = matrix->matrix[i][j];
		}
	}
	printf("Inverted\n");
	PrintMatrix(tempMatrix);
	return NULL;
}

arr* TransposeMatrix(arr* matrix)
{

	return NULL;
}

arr* GaussEliminate(arr* matrix)
{

	//PrintMatrix(idMatrix);
	arr* inMatrix, tnMatrix;
	inMatrix = InvertMatrix(matrix);

	return NULL;
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

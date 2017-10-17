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
	//GaussEliminate(trainMatrix);
	PrintMatrix(trainMatrix);
	Printy();
	Printw();
	Printm();
	return 0;
}

void InvertMatrix(double** matrix)
{

}

double** TransposeMatrix(double** matrix)
{

	return NULL;
}

double** GaussEliminate(double** matrix)
{
	double** idMatrix;
	idMatrix = (double**) malloc((nSize)*sizeof(double*));	// Allocate size
	int rows, cols, i;
	rows = sizeof(matrix)/sizeof(double**);
	cols = sizeof(matrix[0])/sizeof(double*);

	// Allocating space for the training matrix
	for(i = 0; i <= rows; i++)
	{
		idMatrix[i] = (double*) malloc((cols)*sizeof(double));
	}

	rows = rows < cols ? rows : cols;

	for(i = 0; i < rows; i++)
	{
		idMatrix[i][i] = 1.0;
	}

	PrintMatrix(idMatrix);

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


void AssignMatrix()
{
	int i, j;
	for(i = 0; i < matrixSize; i++)
		{
			for(j = 0; j < matrixSize; j++){
				tempMatrix[i][j] = resultMatrix[i][j];
			}
		}
}
 */
void PrintMatrix(double** matrix)
{
	int i, j, rows, cols;
	printf("Matrix\n");
	rows = sizeof(matrix)/sizeof(double*);// / sizeof(matrix[0]);
	cols = sizeof(matrix[0]);// / sizeof(matrix[0][0]);
	printf("rows:%d, cols:%d\n", rows, cols);
	//printf("%f\n", matrix[9][4]);
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 5; j++)
		{
			printf("%f", matrix[i][j]);
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
	for(i = 0; i < mSize; i++)
	{
		for(j = 0; j < kSize; j++)
		{
			printf("%f", testMatrix[i][j]);
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

	trainMatrix = (double**) malloc((nSize)*sizeof(double*));	// Allocate size
	testMatrix = (double**) malloc((mSize)*sizeof(double*));	// Allocate size
	yMatrix = (double*) calloc((nSize),sizeof(double));
	wMatrix = (double*) calloc((kSize+1),sizeof(double));

	// Allocating space for the training matrix
	for(i = 0; i <= nSize; i++)
	{
		trainMatrix[i] = (double*)calloc(kSize+1,sizeof(double));
		trainMatrix[i][0] = 1;
	}

	// Allocate space for test matrix
	for(i = 0; i < mSize; i++)
	{
		testMatrix[i] = (double*)calloc(kSize,sizeof(double));
	}


	// Entering Train values

	for(i = 0; i < nSize; i++)
	{
		for(j = 1; j <= kSize; j++){
			fscanf(fTrain, "%lf,", &temp);
			//printf("temp: %f\n", temp);
			trainMatrix[i][j] = temp;
		}
		fscanf(fTrain, "%lf,", &temp);
		yMatrix[i]= temp;
	}

	// Entering Test values

	for(i = 0; i < mSize; i++)
	{
		for(j = 0; j < kSize; j++){
			fscanf(fTest, "%lf,", &temp);
			//printf("temp: %f\n", temp);
			testMatrix[i][j] = temp;
		}
	}
	fclose(fTrain);
	fclose(fTest);

}

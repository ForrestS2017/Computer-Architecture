#include "learn.h"


int main(int argc, char** argv)
{

	if( argc < 3)
	{
		printf("error");
		exit(0);
	}
	FillMatrix(argc, argv);
	InvertMatrix(trainMatrix);
	PrintMatrix();
	Printy();
	return 0;
}

void InvertMatrix(double** matrix)
{

}

void TransposeMatrix(double** matrix)
{

}

void FillMatrix(int argc, char** argv)
{
	int i, j, r;
	double temp;

	FILE* fp = NULL;
	char* trainingName = NULL;
	char* testingName = NULL;

	trainingName = argv[1];
	testingName = argv[2];

	fp = fopen(trainingName, "r");
	if(fp == NULL){
		printf("error: File does not exist\n");
		exit(0);
	}

	r = fscanf(fp, "%d", &kSize);		// Get size of values
	printf("kSize: %d\n", kSize);
	if(r != 1){printf("error\n"); exit(0);}		// Error throw

	r = fscanf(fp, "%d", &nSize);		// Get size of values
	printf("nSize: %d\n", nSize);

	trainMatrix = (double**) malloc((nSize)*sizeof(double*));	// Allocate size
	yMatrix = (double*) calloc((nSize),sizeof(double));

	// Allocating space for the prim
	for(i = 0; i <= nSize; i++)
	{
		trainMatrix[i] = (double*)calloc(kSize+1,sizeof(double));
		trainMatrix[i][0] = 1;
	}


	// Entering values

	for(i = 0; i < nSize; i++)
	{
		for(j = 1; j <= kSize; j++){
			fscanf(fp, "%lf,", &temp);
			//printf("temp: %f\n", temp);
			trainMatrix[i][j] = temp;
		}
		fscanf(fp, "%lf,", &temp);
		yMatrix[i]= temp;
	}
	//printf("done");

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
void PrintMatrix()
{
	int i, j;
	//printf("%d\n", power);
	for(i = 0; i < nSize; i++)
	{
		for(j = 0; j <= kSize; j++)
		{
			printf("%f", trainMatrix[i][j]);
			printf("\t");
		}
		printf("\n");
	}
}

void Printy()
{
	int i;
	//printf("%d\n", power);
	for(i = 0; i < nSize; i++)
	{
		printf("%f", yMatrix[i]);
		printf("\t");
	}
	printf("\n");

}

void Printw()
{
	int i;
	//printf("%d\n", power);
	for(i = 0; i < nSize; i++)
	{
		printf("%f", wMatrix[i]);
		printf("\t");
	}
	printf("\n");

}

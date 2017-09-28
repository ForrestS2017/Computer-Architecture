
#include <stdio.h>
#include <stdlib.h>

int **matrix = NULL;
int **resultMatrix = NULL;
int **tempMatrix = NULL;
int power, matrixSize;

void FillMatrix(int argc, char** argv);
void MultiplyMatrix();
void AssignMatrix();
void PrintMatrix();
void FreeMemory();

int main(int argc, char** argv)
{

	FillMatrix(argc, argv);
	MultiplyMatrix();
	PrintMatrix();
    FreeMemory();
}

void FillMatrix(int argc, char** argv)
{
	int i, j, r, temp;

	    FILE* fp = NULL;
	    char* filename = NULL;

	    filename = argv[1];

	    fp = fopen(filename, "r");
	    if(fp == NULL){
	        printf("error: File does not exist\n");
	        exit(0);
	    }

	    r = fscanf(fp, "%d", &matrixSize);		// Get size of values
	    //printf("Size: %d, %d\n", matrixSize, r);	//Check it
	    if(r != 1){printf("error\n"); exit(0);}		// Error throw
	    //printf("%d", matrixSize);

	    matrix = (int**) malloc((matrixSize)*sizeof(int*));	// Allocate size
	    resultMatrix = (int**) malloc((matrixSize)*sizeof(int*));
	    tempMatrix = (int**) malloc((matrixSize)*sizeof(int*));

	    // Allocating space for the prim
	    for(i = 0; i < matrixSize; i++)
	    {
	        matrix[i] = (int*)malloc(matrixSize*sizeof(int));
	        resultMatrix[i] = (int*)malloc(matrixSize*sizeof(int));
	        tempMatrix[i] = (int*)malloc(matrixSize*sizeof(int));
	    }

	    // Entering values

	    for(i = 0; i < matrixSize; i++)
	    {
	        for(j = 0; j < matrixSize; j++){
	            fscanf(fp, "%d", &temp);
	            matrix[i][j] = temp;
	            resultMatrix[i][j] = matrix[i][j];
	            tempMatrix[i][j] = matrix[i][j];
	        }
	    }
	    fscanf(fp, "%d", &power);



}

void FreeMemory()
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

void MultiplyMatrix()
{
	int prim, sec, place, inc, temp;
	inc = 0;
	if( power == 0){
		for(place = 0; place < matrixSize; place++)
			{
		        for(sec = 0; sec < matrixSize; sec++){
		            resultMatrix[place][sec] = 1;
		        }
		    }
		return;
	}
	while(power > 1)
	{
		for(place = 0; place < matrixSize; place++, inc++)
		{
			for(prim = 0, temp = 0; prim < matrixSize; prim++, temp = 0)
			{
				for(sec = 0; sec < matrixSize; sec++)
				{
					//printf("p:%d s:%d ", prim, sec);
					temp += tempMatrix[place][sec] * matrix[sec][prim];
				}
				//printf("\n[%d][%d]: %d", place, prim, temp);
				//printf("%d\t ", temp);
				resultMatrix[place][prim] = temp;
			}
		}
		AssignMatrix();
		power--;
	}


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

void PrintMatrix()
{
	int i, j;
	//printf("%d\n", power);
	    for(i = 0; i < matrixSize; i++)
	    {
	        for(j = 0; j < matrixSize; j++)
	        {
	           printf("%d", resultMatrix[i][j]);
	           if(j != matrixSize - 1){
       	        	printf("\t");
       	        }
	        }
	        if(i == matrixSize && j == matrixSize){
	        	printf("\0");
	        	return;
	        }
	        printf("\n");
	    }
}

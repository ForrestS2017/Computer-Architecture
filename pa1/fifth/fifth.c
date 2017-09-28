#include <stdio.h>
#include <stdlib.h>

int readFileName(int argc, char** argv);
int OpenFile(int argc, char** argv);
int VertCheck(int* matrix);
int RowCheck(int* matrix);
int DiagCheck(int* matrix);
int GetInitialTotal(int* matrix);
int matrixSize;
int * values = NULL;
int total;


int main(int argc, char** argv)
{

	if(argc < 2){
		printf("error\n");
		return 0;
	}

	OpenFile(argc, argv);
	GetInitialTotal(values);
	VertCheck(values);
	RowCheck(values);
	DiagCheck(values);
	printf("magic");
	free(values);


	return 0;
}


int VertCheck(int* matrix)
{

	int i, j, temp;
	temp = 0;
		for(i = 0; i < matrixSize; i++, temp = 0){
			for(j = i; j < matrixSize*matrixSize; j += matrixSize){
				//printf("matrix[j] = %d\n", matrix[j]);
				temp += matrix[j];
			}
			//printf("Temp: %d\n", temp);
			if(temp != total)
			{
				printf("not-magic\n");
				exit(0);
			}
		}

	return 1;
}

int RowCheck(int* matrix)
{
	int i, j, temp;
		temp = 0;
		for(i = 0; i < matrixSize*matrixSize; i += matrixSize, temp = 0){
			for(j = i; j < i + matrixSize; j++){
				//printf("matrix[j] = %d\n", matrix[j]);
				temp += matrix[j];
			}
			//printf("Temp: %d\n", temp);
			if(temp != total)
			{
				printf("not-magic\n");
				exit(0);
			}
		}
	return 1;
}

int DiagCheck(int* matrix)
{
	int i = 0;
	int j = 0;
	int temp = 0;
	// Down-Right
		for(i = 0; i < matrixSize*matrixSize; j++, i = j*matrixSize + j){

			//printf("matrix[%d] = %d\n",i , matrix[i]);
			temp += matrix[i];
		}
		//printf("Temp: %d\n", temp);
		if(temp != total)
		{
			printf("not-magic\n");
			exit(0);
		}
	// Down-left
		temp = 0;
		for(i = matrixSize - 1, j = 1;j <= matrixSize ; j++, i = j*(matrixSize - 1) ){

			//printf("matrix[%d] = %d\n",i , matrix[i]);
			temp += matrix[i];
		}
		//printf("Temp: %d\n", temp);
		if(temp != total)
		{
			printf("not-magic\n");
			exit(0);
		}
	return 1;
}

int readFileName(int argc, char** argv){

    /* Goal is to read the file name from the commandline*/
    char* filename;
    filename = argv[1]; //if yes,

    printf("filename entered is %s\n", filename);

    return 0;
}

int OpenFile(int argc, char** argv){
   int i, length, num;

    FILE* fp = NULL;
    char* filename = NULL;

    filename = argv[1];

    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("error: File does not exist\n");
        exit(0);
    }

    int r = fscanf(fp, "%dn", &matrixSize);		// Get size of values
    //printf("Size: %d, %d\n", matrixSize, r);	//Check it
    if(r != 1){printf("error\n"); exit(0);}		// Error throw

    values = (int*) malloc((matrixSize*matrixSize)*sizeof(int));	// Allocate size

    for(i = 0; i < matrixSize * matrixSize; i++)
    {
    	char line[256];
    	fgets(line, sizeof(line), fp);
    	for(i = 0, length = matrixSize*matrixSize; i < length; i++) {
    	        //int curr = line[i]; /* subtracting '0' converts the char to an int */
    	        fscanf(fp, "%d", &num);
    	        //printf("%d\n", num);
    	        values[i] = num;
    	    }
    }
    fclose(fp);

    return 0;
}

int GetInitialTotal(int* matrix)
{
	int i;
	for(i = 0; i < matrixSize; i++){
		total += matrix[i];
	}
	//printf("Initial Total: %d\n", total);
	return total;
}


/*
 *
 * //char line[256];
 *
/*while (fgets(line, sizeof(line), fp))
    {
	if(i == 0)
    	{
    		sscanf(line, "%d", &matrixSize);
    		printf("Size: %d\n", matrixSize);
    		//i++;
    		int* temp[matrixSize][matrixSize];//(int*)malloc(sizeof(matrixSize*matrixSize));
    		*values = (int*)malloc((matrixSize*matrixSize)*sizeof(int)));
    	}
    	printf("%s", line);
    	for(j = 0; j < matrixSize; i++)
    	{
    		values[i] = 5;
    	}
    	printf("logged: %d", &values[i][0]);


    	//    	        //int curr = line[i];  subtracting '0' converts the char to an int

    }*/



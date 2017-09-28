#include <stdlib.h>
#include <stdio.h>

void PrintList(int size);
void Sort(int len);     // Returns node before one you want
void FreeArr();
void ReadFile(int argc, char** argv);  // Reads and inserts or deletes

int* arr = NULL;


int main(int argc, char** argv)
{
	 if(argc < 2)
	 {
	  printf("error\n");
	  exit(0);
	 }

	 ReadFile(argc, argv);

	 FreeArr();

}

void ReadFile(int argc, char** argv)
{
 int r, len, cnt, temp;

    FILE* fp = NULL;
    char* filename = NULL;

    filename = argv[1];

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("error: File does not exist\n");
        exit(0);
    }

    r = fscanf(fp, "%d\n", &len);     // Get operation
    if(r != 1){printf("0\n"); exit(0);}  // Error throw

    arr = (int*) malloc(len * sizeof(int));
    cnt = 0;
    //printf("Length: %d", len);

    while(cnt < len)
    {
    	fscanf(fp, "%d\t", &arr[cnt]);
    	cnt++;
    }

    Sort(len);

    PrintList(len);


}

void Sort(int len)   // Returns node before one you want
{
	int i, temp, j;
	j = 0;

	for(i = 1; i < len; i++, j = i-1)
	{
		temp = arr[i];

		while( j >= 0 && arr[j] > temp)
		{
			arr[j+1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;

	}

}

void PrintList(int size)
{
	int i;
	 //printf("\nLIST: ");


	 for(i = 0; i < size-1; i++)
	 {
	  printf("%d\t", arr[i]);
	 }
	 printf("%d\0", arr[i]);
	 printf("\n");

}

void FreeArr()
{

	free(arr);
}

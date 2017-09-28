#include <stdio.h>
#include <stdlib.h>

void PrintFinal(const char* argv);

int main(int argc, char** argv)
{

	if(argc <= 1){
		return 0;
	}

	int i = 0;
	for(i = 1; i < argc; i++){
		PrintFinal(argv[i]);
	}

   return 0;
}

void PrintFinal(const char* currentWord)
{
	int i = 0;
	while(currentWord[i] != '\0')
	{
		i++;
	}
	printf("%c", currentWord[i-1]);
}

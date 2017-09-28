#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* Compress(char* myString);
void check(char current);
char* compressed;

int main(int argc, char** argv)
{

	if(argc != 2){			//count arguments
		printf("error\n");
		exit(0);
	}

	printf("%s\n", argv[1]);
	printf("%s", Compress(argv[1]));
	free(compressed);
	return 0;
}

char* Compress(char* myString)
{
	int i,j;
	compressed = (char*) malloc(sizeof(myString));	// Allocate space for compressed string

	int amount = 1;
	for(i = 0, j = 0; myString[i] != '\0'; i++, amount++)
	{
		check(myString[i]);								// Check if character is valid
		if( myString[i] != myString[i+1]){				// Check if this character matches next character. If not:
			compressed[j] = myString[i];				// Add current character to result string
			j++;
			compressed[j] = amount + '0';				// Add number to result
			j++;
			amount = 0;									// Reset number
		}
	}
	compressed[j] = '\0';								// Add eol character
	myString[i] = '\0';									// Add eol character
	if(j > i) return myString;							// Return original if shorter
	return compressed;									// Otherwise return compressed
}

void check(char current)
{
	if(current < 58 || current > 127)					// Throw error if character is invalid
	{
		printf("error");
		exit(0);
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef FIRST_H_
#define FIRST_H_

void ReadBuildGraph(int argc, char** argv);
void CalculateOutput();			// Calls Operate() in a loop through all output positions
int Operate(char* target);		// Recursively operates on a single output

int inputCount, inputLength, outputCount, iteration;	// iteration corresponds with the current row of grey code
char** inputs; 	// rows = input index, cols = digits
char** outputs;	// rows = input index, cols = dgits

struct variable* InHead;			// Linked List to hold all INPUT variables
struct variable* OutHead;			// Linked List to hold all OUTPUT variables
struct variable* TempHead;			// Linked List to hold all TEMP variables
struct block* BlockHead;

typedef struct variable
{
	char* name;
	int value;					// 0 or 1, or -1 unset
	struct variable * next;		// Variables will be stored in a linked list
	int type;					// 0 = IN, 1 = OUT, 2 = TEMP
	int order;
	int graycode;

} Variable;

typedef struct block
{
	int type; 			// {1 NOT | 2 AND | 3 OR | 4 NAND | 5 NOR | 6 XOR | 7 DECODER | 8 MULTIPLEXER}
	char** inputs;		// Will hold all inputs in order of entry
	char** outputs;		// Will hold all outputs in order of exit
	char** selectors;	// Will hold all selectors in order of entry
	int inputCount, outputCount, selectorCount;
	struct block* next;
} Block;

Variable* CreateVariable(char* cName, int type)
{
	struct variable* retVar = (struct variable*)malloc(sizeof(struct variable));
	retVar->name = cName;
	retVar->value = -1;		// Initial unevaluated value is -1
	retVar->next = NULL;
	retVar->type = type;
	//retVar->prev = NULL;
	return retVar;
}

Block* CreateBlock(int type, int inSize, int outSize, int selectSize, char** inputs, char** outputs, char** selectors)
{
	struct block* retBlock = (struct block*)calloc(1, sizeof(struct block));
	retBlock->inputCount = inSize;
	retBlock->outputCount = outSize;
	retBlock->selectorCount = selectSize;
	retBlock->type = type;
	retBlock->inputs = inputs;
	retBlock->outputs = outputs;
	retBlock->selectors = selectors;
	retBlock->next = NULL;
	return retBlock;
}

Variable* AddTempVar(char* inName, int type)	// type: 0 = IN, 1 = temp
{
	// If we have no temporary variables, start the list
	if(TempHead != NULL)
	{
		// If we do, traverse our linked list of Temp variables to the end and add
		Variable* tempAdd = (Variable*)malloc(sizeof(Variable));
		tempAdd = CreateVariable(inName, type);

		Variable* traverseHead = TempHead;
		while(traverseHead != NULL)
		{
			// Check we are not adding a duplicate
			if(strcmp(traverseHead->name, inName) == 0)
			{
				return traverseHead;
			}
			// We just add to the end of the list marked by next == NULL
			if(traverseHead->next == NULL)
			{
				traverseHead->next = tempAdd;
				//tempAdd->prev = traverseHead;
				tempAdd->next = NULL;
				return tempAdd;
			}
			traverseHead = traverseHead->next;
		}
	}
	else
	{
		TempHead = (Variable*)malloc(sizeof(Variable));
		TempHead = CreateVariable(inName, type);
		return TempHead;
	}
	return NULL;
}

void AddBlock(int type, int inSize, int outSize, int selectSize, char** inputs, char** outputs, char** selectors)// type: 0 = IN, 1 = temp
{
	// If we have no temporary variables, start the list

	if(BlockHead != NULL)
	{
		Block* tempAdd = (Block*)malloc(sizeof(Block));
		tempAdd = CreateBlock(type, inSize, outSize, selectSize, inputs, outputs, selectors);
		tempAdd->next = BlockHead;
		BlockHead = tempAdd;
		return;
	}
	else
	{
		BlockHead = (Block*)malloc(sizeof(Block));
		BlockHead = CreateBlock(type, inSize, outSize, selectSize, inputs, outputs, selectors);
		return;
	}
}

Variable* SearchVariable(char* name)	// Search for a Variable, using name, input or output
{
	// Type: 0 = name, 1 = input, 2 = output
	Variable* temp;

	// Search in Inputs
	temp = InHead;
	while(temp)
	{
		if(strcmp(temp->name, name) == 0){ return temp;}

		temp = temp->next;
	}
	// Search in Temps
	temp = TempHead;
	while(temp)
	{
		if(strcmp(temp->name, name) == 0) return temp;
		temp = temp->next;
	}
	// Search in Outputs
	temp = OutHead;
	while(temp)
	{
		if(strcmp(temp->name, name) == 0) return temp;
		temp = temp->next;
	}
	return NULL;
}

Block* SearchBlock(char* name, int type)	// Search for a Variable, using name, input or output
{
	// Type: 0 = input, 1 = output, 2 = selector
	Block* temp;

	// Search in Inputs
	switch(type)
	{
	case 0:
		temp = BlockHead;
		while(temp)
		{
			int i = 0;
			int count = temp->inputCount;
			for(i = 0; i < count; i++)
			{
				if(strcmp(temp->inputs[i], name) == 0) return temp;
			}
			temp = temp->next;
		}
		break;
	case 1:
		temp = BlockHead;
		while(temp)
		{
			int i = 0;
			int count = temp->outputCount;
			for(i = 0; i < count; i++)
			{
				if(strcmp(temp->outputs[i], name) == 0) return temp;
			}
			temp = temp->next;
		}
		break;
	case 2:
		temp = BlockHead;
		while(temp)
		{
			int i = 0;
			int count = temp->selectorCount;
			for(i = 0; i < count; i++)
			{
				if(strcmp(temp->selectors[i], name) == 0) return temp;
			}
			temp = temp->next;
		}
		break;
	}
	return NULL;
}

void FillInput( )	// Fill char** input array with grey code sequence
{
	if(!inputCount) return;
	int amount = inputCount;
	int i, j, k, pos, size;
	size = amount;

	int len = 1;				// len is amount of grey code inputs
	while(size >= 1)
	{
		len = len*2;
		size--;
	}
	inputCount = amount;		// Global variable instantiation
	inputLength = len;			// Global variable instantiation

	// Allocate space for input array
	inputs = (char**)malloc(len*sizeof(char*));					// Create a list of length len (2^amount)
	for(i = 0; i < len; i++)
	{
		inputs[i] = (char*)malloc((amount)*sizeof(char));		// Each row should be length of input amount. Each input is a bit
	}

	size = len/2;								// Prep for making half of first row 0's
	for(i = 0; i < amount; i++)					// loop through each column
	{											// High-level row index
		for(pos = 0; pos < len-size;)			// While our index is within bounds
		{
			for(j = 0; j < size; j++)				// j is 0's. j will always start count at current row position
			{
				inputs[pos+j][i] = '0';
			}
			pos += size; if(pos>len-size) break;
			for(k = 0; k < size; k++)				// k is 1's. k will always start count at current row position
			{
				inputs[pos+k][i] = '1';
			}
			pos += size; if(pos>len-size) break; 	//if(pos>len-size) break;
			for(k = 0; k < size; k++)				// k is 1's. k will always start count at current row position
			{
				inputs[pos+k][i] = '1';
			}
			pos += size; if(pos>len-size) break;
			for(j = 0; j < size; j++)				// j is 0's. j will always start count at current row position
			{
				inputs[pos+j][i] = '0';
			}
			pos += size; if(pos>len-size) break;
		}
		size = size/2;
		if(size == 0) break;
	}
	outputs = (char**)malloc(inputLength*sizeof(char*));	// Create the output list of length len (2^amount)
	for(i = 0; i < len; i++)
	{
		outputs[i] = (char*)malloc(outputCount*sizeof(char));		// Allocate space for outputCount output count
		for(k = 0; k < outputCount; k++)
		{
			outputs[i][k] = '0';
		}
	}
}

void ResetValues()	// Reset all input, output values
{
	Variable* temp;

	// Search in Temps
	temp = TempHead;
	while(temp)
	{
		temp->value = -1;
		temp = temp->next;
	}
	// Search in Outputs
	temp = OutHead;
	while(temp)
	{
		temp->value = -1;
		temp = temp->next;
	}
}

void PrintGreyCode()
{
	int i, j;

	for(i = 0; i < inputLength; i++)
	{
		for(j = 0; j < inputCount; j++)
		{
			printf("%c ", inputs[i][j]);
		}
		for(j = 0; j < outputCount; j++)
		{
			printf("%c ", outputs[i][j]);
		}

		printf("\n");

	}
}

void PrintGreyCodeLine(int line)
{
	int  j;


	for(j = 0; j < inputCount; j++)
	{
		printf("%c ", inputs[line][j]);
	}
	for(j = 0; j < outputCount; j++)
	{
		printf("%c ", outputs[line][j]);
	}
	printf("\n");

}

int IntToBinary(int n)
{
	int binaryNumber = 0;
	int remainder, i = 1;

	while (n!=0)
	{
		remainder = n%2;
		n /= 2;
		binaryNumber += remainder*i;
		i *= 10;
	}
	return binaryNumber;
}

int GrayCode(int n)
{
	int grayCode = n ^ (n/ 2);
	return IntToBinary(grayCode); // Converts the num to binary pattern w/o padding
	//return 1;
}

int ToDecimal(int* input, int length)
{
	int result = 0;
	int len = length;
	int i, j;
	for(i = len-1, j=0; i >= 0; i--, j++)
	{
		result += (int)(input[i])*pow(2,j);
	}
	return result;
}

int PIntToInt(int* input, int len)
{
	//printf("Selectors: ");
	int ret = 0;
	int i = 0; int j = 0;
	for(i = len-1; i >= 0; i--, j++)
	{
		ret += input[i]*(int)(pow(10, j) + 0.0001);
	}
	return ret;
}

int GrayToInt(int gray)
{
	int i = 0;
	while(i < 100)
	{
		if(gray == GrayCode(i)) return i;
		i++;
	}
	return 0;
}

#endif /* FIRST_H_ */
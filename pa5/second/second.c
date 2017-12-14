#include "second.h"

int main(int argc, char** argv)
{
	outputCount = 5;
	ReadBuildGraph(argc, argv);
	FillInput();
	CalculateOutput();
	return 0;
}

void CalculateOutput()
{
	int i, k; k = 0;
	Variable* temp;
	for(i = 0; i < inputLength; i++, k = 0)
	{

		// Fill in inputs
		temp = InHead;
		while(temp && k < inputCount)
		{
			temp->value = (int)(inputs[i][k] - 48);
			temp = temp->next;
			k++;
		}
		// Fill in rows of output
		temp = OutHead;
		for(k = 0; k < outputCount; k++)
		{

			outputs[i][k] = Operate(temp->name) + '0';
			temp = temp->next;
			ResetValues();
		}
		PrintGreyCodeLine(i);
	}
}

int Operate(char* target)
{
	/*------ START BASE CASE ------*/
	//  Input Base Case when checked against inputs[order], so return the value if true
	Variable* searchResult = SearchVariable(target);
	if(searchResult->type == 0){ //printf("Input\n");
		return searchResult->value;}
	if(searchResult->value > -1){ //printf("Exists\n");
		return searchResult->value;}
	/*------ END  BASE CASE ------*/

	// Find block that contains the targeted value
	Block* currentBlock = SearchBlock(target, 1);	// Use 1 because our target is the output. From this we can get inputs

	// Check if the values are filled in. if not, recurse!
	int* tempValues = (int*)calloc(currentBlock->inputCount , sizeof(int));		// Create an array to hold all input variable values to easily use
	int* selectValues = (int*)calloc(currentBlock->selectorCount, sizeof(int));	// Create an array to hold all selector variable values to easily use
	int selectorCount = currentBlock->selectorCount;
	int i, temp, k;
	// Actual Evaluation

	for(i = 0, k = 0; i < currentBlock->inputCount; i++, k++)
	{
		// If we have a digit, we don't need to recurse
		if(currentBlock->inputs[i][0] == '0' || currentBlock->inputs[i][0] == '1' )
		{
			tempValues[i] = (int)(currentBlock->inputs[i][0]-48);
		}
		// If we have a variable name, search for it
		else
		{
			searchResult = SearchVariable(currentBlock->inputs[i]);
			temp = searchResult->value;
			// If the variable is not already evaluated, search for it
			if(searchResult->type != 0 && searchResult->value < 0){ temp = Operate(searchResult->name);}
			// Enter the value into our temp array
			searchResult->value = temp;
			tempValues[i] = temp;
			// Process selectors if there are any
		}

		if(selectorCount > 0 && i < currentBlock->selectorCount)
		{
			if(currentBlock->selectors[i][0] == '0' || currentBlock->selectors[i][0] == '1' )
			{
				selectValues[i] = (int)(currentBlock->selectors[i][0]-48);
				continue;
			}
			searchResult = SearchVariable(currentBlock->selectors[i]);
			temp = searchResult->value;
			if(searchResult->type != 0 && searchResult->value < 0) temp = Operate(searchResult->name);
			// Enter the values into our Selector array
			searchResult->value = temp;
			selectValues[i] = temp;
		}

	}

	// Now, we will operate on our tempValues, based on the type # which corresponds to a gate type, and it will be returned
	switch(currentBlock->type)
	{
	case 1:	// NOT
		temp = !tempValues[0];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 2:	// AND
		temp = tempValues[0] && tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 3:	// OR
		temp = tempValues[0] || tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 4:	// NAND
		temp = !(tempValues[0] && tempValues[1]);
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 5:	// NOR
		temp = !(tempValues[0] || tempValues[1]);
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 6:	// XOR
		temp = tempValues[0] ^ tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		return temp;
	case 7:	// DECODER

		// Find what index the grey code is
		for(i = 0; i < currentBlock->outputCount; i++)
		{
			if(strcmp(currentBlock->outputs[i], target) == 0) break;
		}
		// Convert to grey code binary
		temp = GrayCode(i);
		// Check if input is equal to grey code
		i = PIntToInt(tempValues, currentBlock->inputCount);
		if(temp == i) return 1;
		return 0;
	case 8:	// MULTIPLEXER

		// Find input with corresponding graycode
		k = PIntToInt(selectValues, currentBlock->selectorCount);
		for(i = 0; i < currentBlock->inputCount; i++)
		{
			Variable* Temp = SearchVariable(currentBlock->inputs[i]);
			// If our desired input is actually a 1/0, we will get a null TEMP
			if(!Temp)
			{
				temp = GrayToInt(k);
				break;
			}
			// Otherwise, we have a Variable returned, and we can check it's gray code order
			if (Temp->graycode == k)
			{
				temp = i;
				break;
			}
		}
		// Now, get the temp variable w/ corresponding gray code and return it
		temp = tempValues[temp];
		return temp;
	}


	return 0;
}

void ReadBuildGraph(int argc, char** argv)
{
	FILE* fp;
	int i, len;
	Variable* Next;
	//int i, j, inputCount, outputCount;
	i = 0;
	char* tempString = (char*)malloc(32*sizeof(char));
	if(argc < 2)
	{
		printf("error");
		exit(0);
	}

	// Open and read from
	fp = fopen(argv[1], "r");

	//Throw error if no file
	if(fp == NULL)
	{
		printf("error");
		exit(0);
	}
	int r = fscanf(fp, "%s ", tempString);
	if(r == EOF || r == 0)
	{
		printf("error");
		exit(0);
	}
	// Check the file isn't empty and we are starting with INPUTVAR
	while(r != EOF)
	{
		if(r == EOF) break;
		if(strcmp("INPUTVAR", tempString) == 0)
		{
			fscanf(fp, "%d ", &len);
			if(len == 0) return;
			inputCount = len;
			Variable* temp;

			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);

				if(i == 0)
				{
					InHead = (Variable*)malloc(sizeof(Variable));
					InHead->name = inputString;
					InHead->next = NULL;
					InHead->type = 0;
					InHead->order = i;
					continue;
				}
				temp  = InHead;
				while(temp->next) temp = temp->next;

				Next = (Variable*)malloc(sizeof(Variable));
				Next->name = inputString;
				Next->next = NULL;
				Next->type = 0;
				Next->order = i;
				temp->next = Next;
			}


		}
		else if(strcmp("OUTPUTVAR", tempString) == 0)
		{
			fscanf(fp, "%d ", &len);
			outputCount = len;
			Variable* temp;
			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);

				if(i == 0)
				{
					OutHead = (Variable*)malloc(sizeof(Variable));
					OutHead->name = inputString;
					OutHead->value = -1;
					OutHead->next = NULL;
					OutHead->type = 1;
					OutHead->order = i;
					continue;
				}
				temp  = OutHead;
				while(temp->next) temp = temp->next;

				Next = (Variable*)malloc(sizeof(Variable));
				Next->name = inputString;
				Next->value = -1;
				Next->next = NULL;
				Next->type = 1;
				Next->order = i;
				temp->next = Next;
			}
		}
		else if(strcmp("NOT", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 2; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				if(i < 1) inputsForBlock[i] = inputString;
				if(i >= 1) outputsForBlock[0] = inputString;
			}
			AddBlock(1, 1, 1, 0, inputsForBlock, outputsForBlock, NULL);
		}
		else if(strcmp("AND", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(2*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 3; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);

				if(i < 2) inputsForBlock[i] = inputString;
				if(i >= 2) outputsForBlock[0] = inputString;
			}

			AddBlock(2, 2, 1, 0, inputsForBlock, outputsForBlock, NULL);
		}
		else if(strcmp("OR", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(2*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 3; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);

				if(i < 2) inputsForBlock[i] = inputString;
				if(i >= 2) outputsForBlock[0] = inputString;
			}

			AddBlock(3, 2, 1, 0, inputsForBlock, outputsForBlock, NULL);

		}
		else if(strcmp("NAND", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(2*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 3; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);

				if(i < 2) inputsForBlock[i] = inputString;
				if(i >= 2) outputsForBlock[0] = inputString;
			}

			AddBlock(4, 2, 1, 0, inputsForBlock, outputsForBlock, NULL);
		}
		else if(strcmp("NOR", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(2*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 3; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);

				if(i < 2) inputsForBlock[i] = inputString;
				if(i >= 2) outputsForBlock[0] = inputString;
			}

			AddBlock(5, 2, 1, 0, inputsForBlock, outputsForBlock, NULL);

		}
		else if(strcmp("XOR", tempString) == 0)
		{
			// Allocate space for in/out names
			char** inputsForBlock = (char**)malloc(2*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));
			char** outputsForBlock = (char**)malloc(1*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < 3; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);

				if(i < 2) inputsForBlock[i] = inputString;
				if(i >= 2) outputsForBlock[0] = inputString;
			}

			AddBlock(6, 2, 1, 0, inputsForBlock, outputsForBlock, NULL);

		}
		else if(strcmp("DECODER", tempString) == 0)
		{

			// Allocate space for in/out names
			r = fscanf(fp, "%d", &len);
			char** inputsForBlock = (char**)malloc(len*sizeof(char*));
			for(i = 0; i < 2; i++) inputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input
			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				inputsForBlock[i] = inputString;
			}


			// Allocate space for out names
			int len2 = pow(2, len);
			char** outputsForBlock = (char**)malloc(len2*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get outputs
			for(i = 0; i < len2; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				outputsForBlock[i] = inputString;
			}

			AddBlock(7, len, len2, 0, inputsForBlock, outputsForBlock, NULL);

		}
		else if(strcmp("MULTIPLEXER", tempString) == 0)
		{
			// Allocate space for in/out names
			r = fscanf(fp, "%d", &len);
			char** inputsForBlock = (char**)malloc(len*sizeof(char*));
			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				inputsForBlock[i] = inputString;
				if(inputString[0] != '0' && inputString[0] != '1' )SearchVariable(inputString)->graycode = GrayCode(i);
			}

			// Allocate space for selector names
			int len2 = (int)(.000001+log((double)len)/log(2.0));
			char** selectorsForBlock = (char**)malloc(len2*sizeof(char*));
			for(i = 0; i < 1; i++) selectorsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Iterate through line and get selectors
			for(i = 0; i < len2; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				//printf("__%s\n", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				selectorsForBlock[i] = inputString;
			}

			// Allocate space for out names
			char* inputString = (char*)malloc(64*sizeof(char));
			r = fscanf(fp, "%s ", inputString);
			if(inputString[0] >= 'a') AddTempVar(inputString, 2);
			char** outputsForBlock = (char**)malloc(len*sizeof(char*));
			outputsForBlock[0] = inputString;

			AddBlock(8, len, 1, len2, inputsForBlock, outputsForBlock, selectorsForBlock);

		}
		// Get next operation
		r = fscanf(fp, "%s ", tempString);

	}



}

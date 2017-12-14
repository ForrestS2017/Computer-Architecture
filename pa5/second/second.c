#include "second.h"

int main(int argc, char** argv)
{
	outputCount = 5;
	ReadBuildGraph(argc, argv);
	//PrintList();
	//PrintBlocks();
	FillInput();
	CalculateOutput();

	//PrintGreyCode();
	//printf("%lf\n", (log((double)8.0)/log(2.0)));
	return 0;
}

void CalculateOutput()
{
	int i, k; k = 0;
	Variable* temp;
	for(i = 0; i < 3; i++, k = 0)
	{

		iteration = i;
		//outputs[i] = (char*)malloc(outputCount*sizeof(char));		// Allocate space for outputCount output count
		// Fill in inputs
		temp = InHead;
		while(temp && k < inputCount)
		{
			//printf("%c %d\n", inputs[i][k], inputs[i][k]-48);
			temp->value = inputs[i][k] - 48;
			//printf("CHECK %s: %d\n", temp->name, temp->value);
			temp = temp->next;
			k++;
		}
		// Fill in rows of output
		temp = OutHead;
		//ResetValues();
		for(k = 0; k < outputCount; k++)
		{

			printf("==%s\n", temp->name);
			outputs[i][k] = Operate(temp->name) + '0';
			temp = temp->next;
			ResetValues();
		}
	}
}

int Operate(char* target)
{
	//printf("\n");
	/*------ START BASE CASE ------*/
	printf("T: %s\n", target);
	if(target[0] == '0'){return 0;}
	if(target[0] == '1'){return 1;}
	//printf("Target: %s\n", target);
	// Otherwise, we might get a Input Base Case when checked against inputs[order], so return the value if true
	Variable* searchResult = SearchVariable(target);
	//printf("V-Name: %s\t%d\n", searchResult->name, searchResult->value);
	if(searchResult->type == 0) return searchResult->value;
	/*------ END  BASE CASE ------*/

	// Find block that contains the targeted value
	Block* currentBlock = SearchBlock(target, 1);	// Use 1 because our target is the output. From this we can get inputs
	//printf("B-Type: %d\n", currentBlock->type);

	// Check if the values are filled in. if not, recurse!
	int* tempValues = (int*)calloc(currentBlock->inputCount , sizeof(int));	// Create an array to hold all input variable values to easily use
	int* selectValues = (int*)calloc(currentBlock->selectorCount, sizeof(int));
	printf("InSize: %d\n", currentBlock->inputCount);
	int i, temp, count;
	for(i = 0; i < currentBlock->inputCount; i++)
	{
		// If we have a digit, we don't need to recurse
		if(currentBlock->inputs[i][0] == '0' || currentBlock->inputs[i][0] == '1' )
		{
			tempValues[i] = currentBlock->inputs[i][0];
			continue;
		}
		// Otherwise, recurse and get the value
		searchResult = SearchVariable(currentBlock->inputs[i]);
		if(searchResult->type != 0 && searchResult->value < 0){ temp = Operate(searchResult->name);}
		//else{ printf("R-Name: %s\t%d\n", searchResult->name, searchResult->value);}
		temp = searchResult->value;
		tempValues[i] = temp;

		if(currentBlock->selectorCount > 0 && i < currentBlock->selectorCount)
		{
			if(currentBlock->selectors[i][0] == '0' || currentBlock->selectors[i][0] == '1' )
			{
				selectValues[i] = currentBlock->inputs[i][0];
				continue;
			}
			searchResult = SearchVariable(currentBlock->selectors[i]);
			if(searchResult->type != 0 && searchResult->value < 0){ temp = Operate(searchResult->name);}
			//else{ printf("R-Name: %s\t%d\n", searchResult->name, searchResult->value);}

			temp = searchResult->value;
			selectValues[i] = temp;
			printf("ST[]: %d\n",selectValues[i]);
		}
	}
	printf("To_Select: %d %d %d \n", selectValues[0], selectValues[1], selectValues[2]);

	// Now, we will operate on our tempValues, based on the type # which corresponds to a gate type, and it will be returned
	switch(currentBlock->type)
	{
	case 1:	// NOT
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		temp = !tempValues[0];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("1-OUT: %s@%d (!%d)\n", currentBlock->outputs[0], temp, tempValues[0]);
		return temp;
	case 2:	// AND
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		if(tempValues[1] >=48) tempValues[1] = tempValues[0] - 48;
		temp = tempValues[0] && tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("2-OUT: %s@%d (%d && %d)\n", currentBlock->outputs[0], temp, tempValues[0], tempValues[1]);
		return temp;
	case 3:	// OR
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		if(tempValues[1] >=48) tempValues[1] = tempValues[0] - 48;
		temp = tempValues[0] || tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("3-OUT: %s@%d (%d || %d)\n", currentBlock->outputs[0], temp, tempValues[0], tempValues[1]);
		return temp;
	case 4:	// NAND
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		if(tempValues[1] >=48) tempValues[1] = tempValues[0] - 48;
		temp = !(tempValues[0] && tempValues[1]);
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("4-OUT: %s@%d !(%d && %d)\n", currentBlock->outputs[0], temp, tempValues[0], tempValues[1]);
		return temp;
	case 5:	// NOR
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		if(tempValues[1] >=48) tempValues[1] = tempValues[0] - 48;
		temp = !(tempValues[0] || tempValues[1]);
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("5-OUT: %s@%d !(%d || %d)\n", currentBlock->outputs[0], temp, tempValues[0], tempValues[1]);
		return temp;
	case 6:	// XOR
		if(tempValues[0] >=48) tempValues[0] = tempValues[0] - 48;
		if(tempValues[1] >=48) tempValues[1] = tempValues[0] - 48;
		temp = tempValues[0] ^ tempValues[1];
		SearchVariable(currentBlock->outputs[0])->value = temp;
		printf("6-OUT: %s@%d (%d ^ %d)\n", currentBlock->outputs[0], temp, tempValues[0], tempValues[1]);
		return temp;
	case 7:	// DECODER


		break;
	case 8:	// MULTIPLEXER
		count = currentBlock->selectorCount;
		printf("To Select: %d %d %d %d\n", selectValues[0], selectValues[1], selectValues[2], count);
		temp = ToDecimal(selectValues, count);
		printf("Selected: %d\n", temp);
		//temp = tempValues[temp];
		return 1;
		break;
	}
	// If desired value is < 0, go through all inputs

	// if input is non-digit and < 0, set it equal to the Operate()

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
		//printf("error1");
		exit(0);
	}

	// Open and read from
	fp = fopen(argv[1], "r");

	//Throw error if no file
	if(fp == NULL)
	{
		//printf("error2");
		exit(0);
	}
	int r = fscanf(fp, "%s ", tempString);
	if(r == EOF || r == 0)
	{
		//printf("error");
		exit(0);
	}
	// Check the file isn't empty and we are starting with INPUTVAR
	while(r != EOF)
	{
		if(r == EOF) break;
		//printf("\n\n===%s===\n\n", tempString);
		//if(TempHead) //printf("_TempHead: %s\n", TempHead->name);
		if(strcmp("INPUTVAR", tempString) == 0)
		{
			fscanf(fp, "%d ", &len);
			if(len == 0) return;
			//printf("%d\n", len);
			inputCount = len;
			Variable* temp;

			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				//if(InHead)	//printf("HeadName: %s\tTempName: %s\n", InHead->name, inputString);
				r = fscanf(fp, "%s ", inputString);
				//printf("_%s\n", inputString);
				//if(inputString[0] == '\n') break;

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
			//printf("%d\n", len);
			outputCount = len;
			Variable* temp;
			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				//if(OutHead)	//printf("HeadName: %s\tTempName: %s\n", OutHead->name, inputString);
				r = fscanf(fp, "%s ", inputString);
				//printf("_%s\n", inputString);
				//if(inputString[0] == '\n') break;

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
				//printf("_%s", inputString);
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
				//printf("_%s\n", inputString);
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
				//printf("_%s\n", inputString);
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
				//printf("_%s\n", inputString);
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
				//printf("_%s\n", inputString);
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
				//printf("_%s\n", inputString);
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

			// Loop through line and get input/outputs
			for(i = 0; i < len; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				//printf("_%s\n", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				inputsForBlock[i] = inputString;
			}


			// Allocate space for in/out names
			int len2 = pow(2, len);
			char** outputsForBlock = (char**)malloc(len2*sizeof(char*));
			for(i = 0; i < 1; i++) outputsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get input/outputs
			for(i = 0; i < len2; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				//printf("_%s\n", inputString);
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
				//printf("_%s\n", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				inputsForBlock[i] = inputString;
			}

			// Allocate space for in/out names
			int len2 = (int)(.000001+log((double)len)/log(2.0));
			char** selectorsForBlock = (char**)malloc(len2*sizeof(char*));
			for(i = 0; i < 1; i++) selectorsForBlock[i] = (char*)malloc(64*sizeof(char));

			// Loop through line and get selectors
			for(i = 0; i < len2; i++)
			{
				char* inputString = (char*)malloc(64*sizeof(char));
				r = fscanf(fp, "%s ", inputString);
				//printf("_%s\n", inputString);
				if(inputString[0] >= 'a') AddTempVar(inputString, 2);
				selectorsForBlock[i] = inputString;
			}

			// Allocate space for out names
			char* inputString = (char*)malloc(64*sizeof(char));
			r = fscanf(fp, "%s ", inputString);
			//printf("_%s\n", inputString);
			if(inputString[0] >= 'a') AddTempVar(inputString, 2);
			char** outputsForBlock = (char**)malloc(len*sizeof(char*));
			outputsForBlock[0] = inputString;
			////printf("MULTIPLEXXXXXX\n");

			AddBlock(8, len, 1, len2, inputsForBlock, outputsForBlock, selectorsForBlock);

		}
		// Get var count
		r = fscanf(fp, "%s ", tempString);

	}



}


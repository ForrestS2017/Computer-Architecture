#ifndef FIRST_H_
#define FIRST_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct cache* nCache;
struct cache* pCache;
int cachePolicy;		// Used to determine method to increment Line->count;
int counter;			// Global counter to set Line->counter during LRU

void FileOpenCacheSetup(int argc, char** argv);
void PreFetchRW(char** argv);
void NonFetchRW(char** argv);
void PFPrint();
void NFPrint();
void PrintCache(int pf);
void RWEntries(FILE* fp, int sBits, int tBits, int bBits, int pf);	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()


typedef struct cache
{
	int setCount;
	int associativity;	// 0 = direct, 1 = full, 2 = ass
	int setBits;
	int tagBits;
	int blockBits;
	int prefetch;		// 0 = No, 1 = yes;
	//int policy;		// 0 = FIFO; 1 = LRU; Remove since this should be global variable
	int hits, misses, reads, writes;
	struct set** Sets;

} Cache;

typedef struct line
{
	int valid;
	char* tag;
	int count;
} Line;


typedef struct set
{
	int lineCount;
	struct line** Lines;
} Set;

typedef struct address
{
	char* tag;
	char* set;
	char* offset;
} Address;

Line* CreateLine()	//int vBit, unsigned long tBits, int count_in
{
	Line* resLine = (Line*)malloc(sizeof(Line));
	resLine->valid = 0;
	resLine->tag = NULL;
	resLine->count = 0;
	return resLine;

}

Set* CreateSet(int lines)
{
	Set* resStruct = (Set*)malloc(sizeof(Set));
	resStruct->lineCount = lines;
	resStruct->Lines = (Line**)malloc(lines * sizeof(Line*));
	int i=0;
	while(lines >0 && i < resStruct->lineCount)
	{
		resStruct->Lines[i]= CreateLine();
		lines--; i++;
	}
	return resStruct;

}

Cache* CreateCache (int sets, int ass, int sBits, int tBits, int bBits, int isPrefetch, int rAlgo, int lines)
{
	// lines is line per set. sets is amount of sets.
	Cache* resCache = (Cache*)malloc(sizeof(Cache));
	resCache->setCount = sets;
	resCache->associativity = ass;	// 0 = direct, 1 = full, 2 = ass
	resCache->setBits = sBits;
	resCache->tagBits = tBits;
	resCache->blockBits = bBits;
	resCache->prefetch = isPrefetch;
	resCache->hits = 0;
	resCache->misses = 0;
	resCache->reads = 0;
	resCache->writes = 0;
	//resCache->policy = rAlgo;		// 0 = FIFO, 1 = LRU
	resCache->Sets = (Set**) malloc(sets * sizeof(Set*));
	int i=0;
	while(sets >0 && i < resCache->setCount)
	{
		resCache->Sets[i]= CreateSet(lines);
		sets--; i++;
	}
	return resCache;
}

Address* CreateAddress (char* tag, char* index)
{
	Address* retAddress = (Address*)malloc(sizeof(Address));
	retAddress->tag = tag;
	retAddress->set = index;
	retAddress->offset = NULL;
	return retAddress;
}

int IntToBinaryBitLength(long input)
{
	int i = 1;
	while(input > 2)
	{
		input = input >> 1;
		//printf("input: %d", input);
		i++;
	}
	return i;
}

char* ToBinary(char* input){
	int i;
	char* bString = (char*)malloc(48*sizeof(char));

	for(i = 0; i < 12-(strlen(input)); i++){
			strcat(bString,"0000");
	}

	for(i = 0; i < strlen(input); i++){
		switch(input[i]){
		    case '0': strcat(bString,"0000");
			break;
		    case '1': strcat(bString,"0001");
			break;
		    case '2': strcat(bString,"0010");
			break;
		    case '3': strcat(bString,"0011");
			break;
		    case '4': strcat(bString,"0100");
			break;
		    case '5': strcat(bString,"0101");
			break;
		    case '6': strcat(bString,"0110");
			break;
		    case '7': strcat(bString,"0111");
			break;
		    case '8': strcat(bString,"1000");
			break;
		    case '9': strcat(bString,"1001");
			break;
		    case 'a': strcat(bString,"1010");
			break;
		    case 'b': strcat(bString,"1011");
			break;
		    case 'c': strcat(bString,"1100");
			break;
		    case 'd': strcat(bString,"1101");
			break;
		    case 'e': strcat(bString,"1110");
			break;
		    case 'f': strcat(bString,"1111");
			break;
		    case 'A': strcat(bString,"1010");
			break;
		    case 'B': strcat(bString,"1011");
			break;
		    case 'C': strcat(bString,"1100");
			break;
		    case 'D': strcat(bString,"1101");
			break;
		    case 'E': strcat(bString,"1110");
			break;
		    case 'F': strcat(bString,"1111");
			break;
		}
		//strcat(bString," ");
	}



	return bString;
}

int ToDecimal(char* input)
{
	int result = 0;
	int len = strlen(input);
	int i, j;
	for(i = len-1, j=0; i >= 0; i--, j++)
	{
		result += (int)(input[i] - '0')*pow(2,j);
	}
	return result;
}

char* substring(char* input, int start, int length)
{
	int c = 0;
	char* ret = (char*)malloc((length)*sizeof(char));
	while ( c < length )
	{
		ret[c] = input[start+c];
		c++;
	}
	return ret;
}

void printAddress(Address* address)
{
	printf("%s %s %s\n", address->tag, address->set, address->offset);
}


void NFPrint()
{
	printf("no-prefetch\n");
	printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", nCache->reads, nCache->writes, nCache->hits, nCache->misses);
}

void PFPrint()
{
	printf("with-prefetch\n");
	printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", pCache->reads, pCache->writes, pCache->hits, pCache->misses);
}

void PrintCache(int pf)
{
	int sets = pCache->setCount;
	int lines = pCache->Sets[0]->lineCount;
	int i, j;
	if(pf == 0)
	{
		printf("\nvv-----------------Non-Fetch-----------------vv\n");
		for(i = 0; i < sets; i++){
			for(j = 0; j < lines; j++){
				if(nCache->Sets[i]->Lines[j]->valid == 1){
					printf("[%d][%s][Block]--[%d]\n", nCache->Sets[i]->Lines[j]->valid, nCache->Sets[i]->Lines[j]->tag, nCache->Sets[i]->Lines[j]->count);
				}else{
					printf("[0][empty][null]--[unset]\n");
				}
			}
			printf("-----------------------------------\n");
		}
		printf("^^-----------------Non-Fetch-----------------^^\n");
	}
	else
	{
		printf("\nvv-----------------Pre-Fetch-----------------vv\n");
		for(i = 0; i < sets; i++){
			for(j = 0; j < lines; j++){
				if(pCache->Sets[i]->Lines[j]->valid == 1){
					printf("[%d][%s][Block]--[%d]\n", pCache->Sets[i]->Lines[j]->valid, pCache->Sets[i]->Lines[j]->tag, pCache->Sets[i]->Lines[j]->count);
				}else{
					printf("[0][empty][null]--[unset]\n");
				}
			}
			printf("-----------------------------------\n");
		}
		printf("^^-----------------Pre-Fetch-----------------^^\n");

	}


}


#endif /* FIRST_H_ */
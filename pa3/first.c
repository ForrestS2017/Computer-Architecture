#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "first.h"

int main(int argc, char** argv)
{

	FileOpenCacheSetup(argc, argv);
	//PreFetchRW(argv);
	NonFetchRW(argv);
	//NFPrint();
	//PFPrint();
	PrintCache(0);
	//printf("%s", ToBinary("999999999999"));
	printf("Tag Bits:\t%d\nSet Bits:\t%d\nBlock Bits:\t%d", pCache->tagBits, pCache->setBits, pCache->blockBits );
	return 0;
}

void PreFetchRW(char** argv)
{
	FILE *fp;
	fp = fopen(argv[5], "r");
	//int hexAddress;
	//printf("\nSets: %d, Lines: %d\n", pCache->setCount, pCache->Sets[0]->lineCount);

	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()
	RWEntries(fp, pCache->setBits, pCache->tagBits, pCache->blockBits);
	fclose(fp);
}

void NonFetchRW(char** argv)
{
	FILE *fp;
	fp = fopen(argv[5], "r");
	//int hexAddress;
	//printf("\nSets: %d, Lines: %d\n", pCache->setCount, pCache->Sets[0]->lineCount);

	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()
	RWEntries(fp, nCache->setBits, nCache->tagBits, nCache->blockBits);
	fclose(fp);
}

void RWEntries(FILE* fp, int sBits, int tBits, int bBits)
{
	int r = 1;
	char op;
	char* bAddress = (char*)calloc(48, sizeof(char));
	char* hexAddress = (char*)calloc(12, sizeof(char));
	int pc;

	// We must read all entries, so loop through the entire document
	// eg: 0x804ae19: R 0x9cb3d40
	while(r != EOF)
	{
		fscanf(fp, "%x: ", &pc);
		fscanf(fp, "%c ", &op);
		if(op == '#') break;
		fscanf(fp, "%s\n", hexAddress);
		bAddress = ToBinary(hexAddress);
		printf("%x: %c %s\t%s\n", pc, op, hexAddress, bAddress);
	}



}

void FileOpenCacheSetup(int argc, char** argv)
{
	// ./first <cache size> <associativity> <cache policy> <block size> <trace file>
	FILE *fp;
	int cSize, sets, ass, sBits, tBits, bBits, lines;

	// Throw error if insufficient argument count
	if(argc < 6)
	{
		printf("error");
		exit(0);
	}

	// Open and read from
	fp = fopen(argv[5], "r");

	//Throw error if no file
	if(fp == NULL)
	{
		printf("error");
		exit(0);
	}

	// Check Cache Policy
	if(strncmp(argv[3], "fifo", 4) == 0)
	{
		cachePolicy = 0;
	}else if(strncmp(argv[3], "lru", 3) == 0)
	{
		cachePolicy = 1;
	}

	// Get block size in bytes, convert to bits
	bBits = atoi(argv[4]) * 8;

	// Get cache size in bytes, convert to bits
	cSize = atoi(argv[1]) * 8;

	// Check Association
	if(strcmp(argv[2], "direct") == 0){		// Direct-Mapped
		ass = 0;
		lines = 1;
		sets = cSize/bBits;
	}else if(strcmp(argv[2], "assoc") == 0){	// Fully Associative
		ass = 1;
		lines = cSize/bBits;
		sets = 1;
	}else{									// Partial Associative. argv[2][6] gives lines per set
		ass = 2;
		lines = argv[2][6] - '0';
		sets = cSize/(bBits*lines);
	}

	fclose(fp);

	//Get length of set index
	sBits = IntLength(sets);
	tBits = 48 - sBits - bBits;

	//Create two separate Caches, one w/ and one w/o pre-fetch. Non-pre-fetch will run first in main();
	nCache = CreateCache(sets, ass, sBits, tBits, bBits, 0, cachePolicy, lines);
	pCache = CreateCache(sets, ass, sBits, tBits, bBits, 1, cachePolicy, lines);

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
		printf("vv-----------------Non-Fetch-----------------vv\n");
		for(i = 0; i < sets; i++){
			for(j = 0; j < lines; j++){
				if(nCache->Sets[i]->Lines[j]->valid == 1){
					printf("[%d][%lu][Block]--[%d]\n", nCache->Sets[i]->Lines[j]->valid, nCache->Sets[i]->Lines[j]->tag, nCache->Sets[i]->Lines[j]->count);
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
		printf("vv-----------------Pre-Fetch-----------------vv\n");
		for(i = 0; i < sets; i++){
			for(j = 0; j < lines; j++){
				if(pCache->Sets[i]->Lines[j]->valid == 1){
					printf("[%d][%lu][Block]--[%d]\n", pCache->Sets[i]->Lines[j]->valid, pCache->Sets[i]->Lines[j]->tag, pCache->Sets[i]->Lines[j]->count);
				}else{
					printf("[0][empty][null]--[unset]\n");
				}
			}
			printf("-----------------------------------\n");
		}
		printf("^^-----------------Pre-Fetch-----------------^^\n");

	}


}

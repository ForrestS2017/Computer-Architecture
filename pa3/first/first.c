#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "first.h"

void PFRWData(Address* address, char op);	// Pre-Fetch Read-Write Data
void NFRWData(Address* address, char op);	// No-Fetch Read-Write Data

int main(int argc, char** argv)
{

	FileOpenCacheSetup(argc, argv);
	printf("Tag Bits:\t%d\nSet Bits:\t%d\nBlock Bits:\t%d\nSet Count:\t%d\n", pCache->tagBits, pCache->setBits, pCache->blockBits, pCache->setCount );
	PreFetchRW(argv);
	NonFetchRW(argv);
	//NFPrint();
	//PFPrint();
	PrintCache(0);
	//printf("%s", DecimalToBinary("999999999999"));
	//printf("D: %d\n", ToDecimal("100110010"));
	return 0;
}

void PreFetchRW(char** argv)
{
	FILE *fp;
	fp = fopen(argv[5], "r");
	//int hexAddress;
	//printf("\nSets: %d, Lines: %d\n", pCache->setCount, pCache->Sets[0]->lineCount);

	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()
	RWEntries(fp, pCache->setBits, pCache->tagBits, pCache->blockBits, 1);
	fclose(fp);
}

void NonFetchRW(char** argv)
{
	FILE *fp;
	fp = fopen(argv[5], "r");

	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()
	RWEntries(fp, nCache->setBits, nCache->tagBits, nCache->blockBits, 0);
	fclose(fp);
}

void RWEntries(FILE* fp, int sBits, int tBits, int bBits, int pf)
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
		//printf("%x: %c %s\t%s\n", pc, op, hexAddress, bAddress);
		Address* address = CreateAddress(substring(bAddress, 0, nCache->tagBits), substring(bAddress, nCache->tagBits, nCache->setBits));
		printAddress(address);

		switch(pf)
		{
		case 0:
			PFRWData(address, op);
			break;
		case 1:
			NFRWData(address, op);
			break;
		}
	}
}

void PFRWData(Address* address, char op)
{

}

void NFRWData(Address* address, char op)
{
	if(op == 'W') goto _Write;

	// Read
	int set = ToDecimal(address->set);
	counter++;
	return;

	// Write
	_Write:

	counter++;
}

void FileOpenCacheSetup(int argc, char** argv)
{
	// ./first <cache size> <associativity> <cache policy> <block size> <trace file>
	FILE *fp;
	int cSize, setCountS, ass, sBits, tBits, bBits, lineCountE, blockSizeB;

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
	blockSizeB = pow(2, atoi(argv[4]));

	// Get cache size in bytes, convert to bits
	cSize = atoi(argv[1]) * 8;
	printf("Cache Bits:\t%d\n", cSize);
	printf("Block Size:\t%d\n", blockSizeB);

	// Check Association
	// Get set count as well. Cache Size = bytes per Block * Set count * Lines per Set
	if(strcmp(argv[2], "direct") == 0){		// Direct-Mapped
		ass = 0;
		lineCountE = 1;
		setCountS = cSize/(blockSizeB*lineCountE);
	}else if(strcmp(argv[2], "assoc") == 0){	// Fully Associative
		ass = 1;
		lineCountE = cSize/blockSizeB;
		setCountS = 1;
	}else{									// Partial Associative. argv[2][6] gives lineCountE per set
		ass = 2;
		lineCountE = argv[2][6] - '0';
		setCountS = cSize/(blockSizeB*lineCountE);
	}

	fclose(fp);

	//Get length of set index
	sBits = IntToBinaryBitLength(setCountS);
	bBits = IntToBinaryBitLength(blockSizeB);
	tBits = 48 - sBits - bBits;

	//Create two separate Caches, one w/ and one w/o pre-fetch. Non-pre-fetch will run first in main();
	nCache = CreateCache(setCountS, ass, sBits, tBits, bBits, 0, cachePolicy, lineCountE);
	pCache = CreateCache(setCountS, ass, sBits, tBits, bBits, 1, cachePolicy, lineCountE);

}

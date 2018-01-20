#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "first.h"

void PFRWData(Address* address, char op);	// Pre-Fetch Read-Write Data FIFO
void NFRWData(Address* address, char op);	// No-Fetch Read-Write Data FIFO
void PreFetch(Address* address);

int main(int argc, char** argv)
{

	FileOpenCacheSetup(argc, argv);
	NonFetchRW(argv);
	PreFetchRW(argv);
	NFPrint();
	PFPrint();
	return 0;
}

void PreFetchRW(char** argv)	// pf == 1
{
	counter = 0;
	FILE *fp;
	fp = fopen(argv[5], "r");
	//int hexAddress;
	//printf("\nSets: %d, Lines: %d\n", pCache->setCount, pCache->Sets[0]->lineCount);

	//Take care of reading the file in this method. Delegated so we can reuse in NonFetchRW()
	RWEntries(fp, pCache->setBits, pCache->tagBits, pCache->blockBits, 1);
	fclose(fp);
}

void NonFetchRW(char** argv)	//pf = 0
{
	counter = 0;
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
	char* bAddress = (char*)calloc(49, sizeof(char));
	char* hexAddress = (char*)calloc(13, sizeof(char));
	Address* address = NULL;
	int pc;

	// We must read all entries, so loop through the entire document
	// eg: 0x804ae19: R 0x9cb3d40
	while(r != EOF)
	{
		fscanf(fp, "%x: ", &pc);
		fscanf(fp, "%c ", &op);
		if(op == '#') break;

		fscanf(fp, "0x%s\n", hexAddress);
		bAddress = ToBinary(hexAddress);
		address = CreateAddress(substring(bAddress, 0, nCache->tagBits), substring(bAddress, nCache->tagBits, nCache->setBits), hexAddress);
		//printf("%c: ", op); printAddress(address);

		switch(pf)
		{
		case 0:
			NFRWData(address, op);
			break;
		case 1:
			PFRWData(address, op);
			break;
		}
	}
}

void PFRWData(Address* address, char op)
{
	if( !address) return;
	int set, lineCount, i;	// Declare variables to be used
	char* tag;
	char* tempTag;
	Set* cSet;
	Line* cLine;
	if(op == 'W') goto _Write;

	// --Read--

	set = ToDecimal(address->set);
	tag = address->tag;
	tempTag = NULL;
	cSet = pCache->Sets[set];
	lineCount = pCache->Sets[0]->lineCount;
	cLine = NULL;
	// Look for line
	for( i = 0; i < lineCount; i++)
	{
		if(cSet->Lines[i]->valid == 1)
		{	// If valid == 0, continue to prevent n-pointer issues
			tempTag = cSet->Lines[i]->tag;
			// Check the hit
			if(strcmp(tempTag, tag) == 0)
			{
				// Update counters
				if(cachePolicy == 1)
				{
					cSet->Lines[i]->count = counter;
					counter++;
				}
				pCache->hits++;
				return;
			}
		}
	}

	// If we got a miss

	if(cLine == NULL)
	{
		pCache->misses++;
		pCache->reads++;
		cLine = cSet->Lines[0];

		// Search for the oldest one
		for(i = 0; i < lineCount; i++)
		{
			// Get lowest count line
			if(cSet->Lines[i]->count <= cLine->count ) cLine = cSet->Lines[i];
			// If we find an empty line
			if(cSet->Lines[i]->valid == 0)
			{
				cSet->Lines[i]->valid = 1;
				cSet->Lines[i]->tag = tag;
				cSet->Lines[i]->count = counter;
				if(cachePolicy == 0) counter++;
				PreFetch(address);
				return;
			}
		}

		// We did not find empty spot, so replace cLine, which is now the lowest one.
		cLine->tag = address->tag;
		cLine->valid = 1;
		cLine->count = counter;
		counter++;
		PreFetch(address);

		return;
	}


	return;

	// --Write--
	_Write:

	set = ToDecimal(address->set);
	tag = address->tag;
	tempTag = NULL;
	cSet = pCache->Sets[set];
	lineCount = pCache->Sets[0]->lineCount;
	cLine = NULL;
	pCache->writes++;

	// Look for line
	for( i = 0; i < lineCount; i++)
	{
		if(cSet->Lines[i]->valid == 1)
		{	// If valid == 0, continue to prevent n-pointer issues
			tempTag = cSet->Lines[i]->tag;
			// Check the hit
			if(strcmp(tempTag, tag) == 0)
			{
				// Update counters
				if(cachePolicy == 1)
				{
					cSet->Lines[i]->count = counter;
					counter++;
				}
				pCache->hits++;
				return;
			}
		}
	}

	// If we got a miss
	if(cLine == NULL)
	{
		pCache->misses++;
		pCache->reads++;
		cLine = cSet->Lines[0];

		// Search for the oldest one
		for(i = 0; i < lineCount; i++)
		{
			// Get lowest count line
			if(cSet->Lines[i]->count <= cLine->count ) cLine = cSet->Lines[i];
			// If we find an empty line
			if(cSet->Lines[i]->valid == 0)
			{
				cSet->Lines[i]->valid = 1;
				cSet->Lines[i]->tag = tag;
				cSet->Lines[i]->count = counter;
				if(cachePolicy == 0) counter++;
				PreFetch(address);
				return;
			}
		}

		// We did not find empty spot, so replace cLine, which is now the lowest one.
		cLine->tag = address->tag;
		cLine->valid = 1;
		cLine->count = counter;
		counter++;
		PreFetch(address);
		return;
	}

	return;
}

void NFRWData(Address* address, char op)
{
	if( !address) return;
	int set, lineCount, i;	// Declare variables to be used
	char* tag = NULL;
	char* tempTag = NULL;
	Set* cSet = NULL;
	Line* cLine = NULL;
	if(op == 'W') goto _Write;

	// --Read--

	set = ToDecimal(address->set);
	tag = address->tag;
	tempTag = NULL;
	cSet = nCache->Sets[set];
	lineCount = nCache->Sets[0]->lineCount;
	cLine = NULL;
	// Look for line
	for( i = 0; i < lineCount; i++)
	{
		if(cSet->Lines[i]->valid == 1)
		{	// If valid == 0, continue to prevent n-pointer issues
			tempTag = cSet->Lines[i]->tag;
			// Check the hit
			if(strcmp(tempTag, tag) == 0)
			{
				// Update counters
				if(cachePolicy == 1)
				{
					cSet->Lines[i]->count = counter;
					counter++;
				}
				nCache->hits++;
				return;
			}
		}
	}

	// If we got a miss

	if(cLine == NULL)
	{
		nCache->misses++;
		nCache->reads++;
		cLine = cSet->Lines[0];

		// Search for the oldest one
		for(i = 0; i < lineCount; i++)
		{
			// Get lowest count line
			if(cSet->Lines[i]->count <= cLine->count ) cLine = cSet->Lines[i];
			// If we find an empty line
			if(cSet->Lines[i]->valid == 0)
			{
				cSet->Lines[i]->valid = 1;
				cSet->Lines[i]->tag = tag;
				cSet->Lines[i]->count = counter;
				if(cachePolicy == 0) counter++;
				return;
			}
		}

		// We did not find empty spot, so replace cLine, which is now the lowest one.
		cLine->tag = address->tag;
		cLine->valid = 1;
		cLine->count = counter;
		counter++;

		return;
	}


	return;

	// --Write--
	_Write:

	set = ToDecimal(address->set);
	tag = address->tag;
	tempTag = NULL;
	cSet = nCache->Sets[set];
	lineCount = nCache->Sets[0]->lineCount;
	cLine = NULL;
	nCache->writes++;

	// Look for line
	for( i = 0; i < lineCount; i++)
	{
		if(cSet->Lines[i]->valid == 1)
		{	// If valid == 0, continue to prevent n-pointer issues
			tempTag = cSet->Lines[i]->tag;
			// Check the hit
			if(strcmp(tempTag, tag) == 0)
			{
				// Update counters
				if(cachePolicy == 1)
				{
					cSet->Lines[i]->count = counter;
					counter++;
				}
				nCache->hits++;
				return;
			}
		}
	}

	// If we got a miss
	if(cLine == NULL)
	{
		nCache->misses++;
		nCache->reads++;
		cLine = cSet->Lines[0];

		// Search for the oldest one
		for(i = 0; i < lineCount; i++)
		{
			// Get lowest count line
			if(cSet->Lines[i]->count <= cLine->count ) cLine = cSet->Lines[i];
			// If we find an empty line
			if(cSet->Lines[i]->valid == 0)
			{
				cSet->Lines[i]->valid = 1;
				cSet->Lines[i]->tag = tag;
				cSet->Lines[i]->count = counter;
				if(cachePolicy == 0) counter++;
				return;
			}
		}

		// We did not find empty spot, so replace cLine, which is now the lowest one.
		cLine->tag = address->tag;
		cLine->valid = 1;
		cLine->count = counter;
		counter++;

		return;
	}

	return;

}

void PreFetch(Address* address)
{
	if( !address) return;
	int set, lineCount, i;	// Declare variables to be used
	char* tag;
	char* tempTag;
	Set* cSet;
	Line* cLine;
	int hex = strtol(address->hex, NULL, 16);
	hex = hex + pCache->blockSize/8;
	char* bAddress = (char*)malloc(48 * sizeof(char));
	sprintf(bAddress, "%x", hex);
	bAddress = ToBinary(bAddress);
	address = CreateAddress(substring(bAddress, 0, nCache->tagBits), substring(bAddress, nCache->tagBits, nCache->setBits), "");

	set = ToDecimal(address->set);
	tag = address->tag;
	tempTag = NULL;
	cSet = pCache->Sets[set];
	lineCount = pCache->Sets[0]->lineCount;
	cLine = NULL;
	// Look for line
	for( i = 0; i < lineCount; i++)
	{
		if(cSet->Lines[i]->valid == 1)
		{	// If valid == 0, continue to prevent n-pointer issues
			tempTag = cSet->Lines[i]->tag;
			// Check the hit
			if(strcmp(tempTag, tag) == 0)
			{
				return;
			}
		}
	}

	// If we got a miss

	if(cLine == NULL)
	{
		pCache->reads++;
		cLine = cSet->Lines[0];

		// Search for the oldest one
		for(i = 0; i < lineCount; i++)
		{
			// Get lowest count line
			if(cSet->Lines[i]->count <= cLine->count ) cLine = cSet->Lines[i];
			// If we find an empty line
			if(cSet->Lines[i]->valid == 0)
			{
				cSet->Lines[i]->valid = 1;
				cSet->Lines[i]->tag = tag;
				//if(cachePolicy == 0) cSet->Lines[i]->count = counter;
				//counter++;
				cSet->Lines[i]->count = counter;
				if(cachePolicy == 0) counter++;
				return;
			}
		}

		// We did not find empty spot, so replace cLine, which is now the lowest one.
		cLine->tag = address->tag;
		cLine->valid = 1;
		cLine->count = counter;
		counter++;

		return;
	}
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
	blockSizeB = atoi(argv[4])*8;

	// Get cache size in bytes, convert to bits
	cSize = atoi(argv[1]) * 8;

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
	sBits = (int)(log((double)setCountS)/log(2.0));
	bBits = (int)(log((double)(blockSizeB/8))/log(2.0));
	tBits = 48 - sBits - bBits;

	//Create two separate Caches, one w/ and one w/o pre-fetch. Non-pre-fetch will run first in main();
	nCache = CreateCache(setCountS, ass, sBits, tBits, bBits, blockSizeB, 0, cachePolicy, lineCountE);
	pCache = CreateCache(setCountS, ass, sBits, tBits, bBits, blockSizeB, 1, cachePolicy, lineCountE);

}


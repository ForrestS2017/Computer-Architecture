#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//FUNCTION DECLARATIONS
char* toBinary(char* mem);
int toDec(char* input);
char* subString(char* input, int start, int end);
int printCache(int numSets, int numLines);
char* prefetchCalc(char* mem, int size);
long long prefetch(long long counter, long tagLen, char* binary2, int numLines, int setIndex2);

//LINE STRUCTURE -> A single cache line containing valid bit, tag bits, block bits (unused for this project), and counter for fifo/lru.
struct line{
	int valid;
	char* tag;
	char* block;
	long ageFreq;
};

//L1 CACHE 2D ARRAY -> The actual cache structure for the project: an array of sets (which are arrays of lines) therefore a 2-d line struct array.
struct line** cache;

//TRACKERS -> counts for operations
int readsM; //memory
int writesM; //memory
int hitsC; //cache
int missesC; //cache

//CACHE POLICY -> fifo/lru
int policy;

//CACHE PRINTER -> printing helper method: prints cache in practical format.
int printCache(int numSets, int numLines){
	int i, j;

	printf("-----------------------------------\n");
	for(i = 0; i < numSets; i++){
		for(j = 0; j < numLines; j++){
			if(cache[i][j].valid == 1){
				printf("[%d][%s][Block]--[%li]\n", cache[i][j].valid, cache[i][j].tag, cache[i][j].ageFreq);
			}else{
				printf("[0][empty][null]--[unset]\n");
			}
		}
		printf("-----------------------------------\n");
	}

	return 0;
}

//HEX TO BINARY -> conversion helper method: mainly for conversion of memory address strings to binary strings of 48 bit.
char* toBinary(char* input){
	int i;
	char* ret = (char*)malloc(64*sizeof(char));

	for(i = 0; i < 12-(strlen(input)-2); i++){
		strcat(ret,"0000");
	}

	for(i = 2; i < strlen(input); i++){
		switch(input[i]){
		    case '0': strcat(ret,"0000");
			break;
		    case '1': strcat(ret,"0001");
			break;
		    case '2': strcat(ret,"0010");
			break;
		    case '3': strcat(ret,"0011");
			break;
		    case '4': strcat(ret,"0100");
			break;
		    case '5': strcat(ret,"0101");
			break;
		    case '6': strcat(ret,"0110");
			break;
		    case '7': strcat(ret,"0111");
			break;
		    case '8': strcat(ret,"1000");
			break;
		    case '9': strcat(ret,"1001");
			break;
		    case 'a': strcat(ret,"1010");
			break;
		    case 'b': strcat(ret,"1011");
			break;
		    case 'c': strcat(ret,"1100");
			break;
		    case 'd': strcat(ret,"1101");
			break;
		    case 'e': strcat(ret,"1110");
			break;
		    case 'f': strcat(ret,"1111");
			break;
		    case 'A': strcat(ret,"1010");
			break;
		    case 'B': strcat(ret,"1011");
			break;
		    case 'C': strcat(ret,"1100");
			break;
		    case 'D': strcat(ret,"1101");
			break;
		    case 'E': strcat(ret,"1110");
			break;
		    case 'F': strcat(ret,"1111");
			break;
		}
	}

	return ret;
}



//BINARY TO DECIMAL -> conversion helper method: mainly for conversion of set bits in binary address into decimal value to identify relevant cache set
int toDec(char* input){
	int ret = 0;
	int base = 1;
	int len = strlen(input);
	int i;

	for(i = len-1; i >= 0; i--){
		if(input[i] == '0'){
			ret += 0 * base;
			base = base * 2;
		}else{
			ret += 1 * base;
			base = base * 2;
		}
	}

	return ret;
}

//SUBSTRING SELECTOR -> helper method: takes input string and returns substring including starting position index and up until but not including ending position index.
char* subString(char* input, int start, int end){
	char* ret = (char*)malloc((64)*sizeof(char));
	int i;

	for(i = 0; i < (end-start); i++){
		ret[i] = input[i+start];
	}

	return ret;
}

//PREFETCH CALCULATOR -> calculates memory address to be prefetched
char* prefetchCalc(char* mem, int size){
	int i, value, len;
	long long decimal = 0;

	len = strlen(mem);
	len--;

	for(i = 0; i < strlen(mem); i++){
		if(mem[i] >= '0' && mem[i] <= '9'){
			value = mem[i] - 48;
		}else if(mem[i] >= 'a' && mem[i] <= 'f'){
			value = mem[i] - 97 + 10;
		}else if(mem[i] >= 'A' && mem[i] <= 'F'){
			value = mem[i] - 65 + 10;
		}

		decimal += value * pow(16, len);
		len--;
	}

	decimal += size;

	long long quot, rem;
	int j = 0;
	char* ret = (char*)malloc(100*sizeof(char));

	quot = decimal;

	while(quot != 0){
		rem = quot % 16;
		if(rem < 10){
			ret[j++] = 48 + rem;
		}else{
			ret[j++] = 87 + rem;
		}
		quot = quot/16;
	}

	char temp;
	strcat(ret,"x0");

	for(i = 0; i < strlen(ret)/2; i++){
		temp = ret[i];
		ret[i] = ret[strlen(ret)-i-1];
		ret[strlen(ret)-i-1] = temp;
	}

	return ret;
}

//PREFETCHING METHOD -> adds prefetched block to cache and updates
long long prefetch(long long counter, long tagLen, char* binary2, int numLines, int setIndex2){
	int i;
	for(i = 0; i < numLines; i++){
		if(cache[setIndex2][i].valid == 1){
			if(strncmp(cache[setIndex2][i].tag, subString(binary2, 0, tagLen), tagLen) == 0){
				return counter;
			}
		}
	}

	readsM++;

	for(i = 0; i < numLines; i++){
			if(cache[setIndex2][i].valid == 0){
			cache[setIndex2][i].valid = 1;
			cache[setIndex2][i].tag = subString(binary2, 0, tagLen);
			cache[setIndex2][i].ageFreq = counter;
			if(policy == 0){
				counter++;
			}
			return counter;
		}
	}

	int repIndex = 0;
	for(i = 0; i < numLines; i++){
		if(cache[setIndex2][i].valid == 1){
			if(cache[setIndex2][i].ageFreq <= cache[setIndex2][repIndex].ageFreq){
				repIndex = i;
			}
		}
	}

	cache[setIndex2][repIndex].valid = 1;
	cache[setIndex2][repIndex].tag = subString(binary2, 0, tagLen);
	cache[setIndex2][repIndex].ageFreq = counter;
	counter++;

	return counter;
}

//MAIN METHOD -> intakes arguements and sets up program memory with mallocing of structs and variables. Also contains main program while loop with cache functions.
int main(int argc, char** argv){

	//Opening trace file and checking for sufficient number of arguments
	FILE* fp;

	if(argc < 6){
		printf("error");
		return 0;
	}

	fp = fopen(argv[5], "r");

	if(fp == NULL){
		printf("error");
		return 0;
	}

	//Initializing Total Cache Size IN BITS
	int cacheSize = 8 * atoi(argv[1]);

	//Initializing Cache Policy -- default fifo = 0 unless lru = 1 detected
	policy = 0;

	if(strncmp(argv[3], "lru", 3) == 0){
		policy = 1;
	}

	//Initializing Total Block Size IN BITS
	int blockSize = 8 * atoi(argv[4]);

	//Initializing Associativity -- number of sets to consider & cacheSize = blockSize x numLines x numSets
	int numSets = 0;
	int numLines = 1;

	if(strcmp(argv[2], "direct") == 0){
		numSets = cacheSize/blockSize;
		numLines = 1;
	}else if(strcmp(argv[2], "assoc") == 0){
		numSets = 1;
		numLines = cacheSize/blockSize;
	}else{
		numLines = argv[2][6]-'0';
		numSets = cacheSize/(blockSize*numLines);
	}

	//Initializing Cache (an array of set arrays of length numLines) and set all valid bits to 0
	long i,j;

	cache = (struct line**)malloc(numSets*sizeof(struct line*));
	for(i = 0; i < numSets; i++){
		cache[i] = (struct line*)malloc(numLines*sizeof(struct line));
	}

	for(i = 0; i < numSets; i++){
		for(j = 0; j < numLines; j++){
			cache[i][j].valid = 0;
		}
	}

	//Initializing trackers to be printed
	readsM = 0;
	writesM = 0;
	hitsC = 0;
	missesC = 0;

	//Begin loop for processing addresses
	char* pc = (char*)malloc(69*sizeof(char));
	char* rw = (char*)malloc(10*sizeof(char));
	char* mem = (char*)malloc(16*sizeof(char));
	char* binary;

	int setSize = (int)(log((double)numSets)/log(2.0)); //IN BITS
	int blockBits = (int)(log((double)(blockSize/8))/log(2.0));
	long tagLen = 48-blockBits-setSize;
	int setIndex;
	long long counter = 0;
	int repIndex = 0;
	long lc = 0;

	LOOP:do{
		lc++;
		fscanf(fp, "%s: ", pc);
		fscanf(fp, "%s ", rw);
		fscanf(fp, "%s\n", mem);

		if(strncmp(pc, "#eof", 4) == 0){
			break;
		}

		binary = toBinary(mem);
		setIndex = toDec(subString(binary, tagLen, (48-blockBits)));

		if(strcmp(rw, "R") == 0){
			//printf("R %s - %ld\n", mem, lc);
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(strncmp(cache[setIndex][i].tag, subString(binary, 0, tagLen), tagLen) == 0){
						hitsC++;
						if(policy == 1){
							cache[setIndex][i].ageFreq = counter;
							counter++;
						}
						goto LOOP;
					}
				}
			}

			missesC++;
			readsM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 0){
					cache[setIndex][i].valid = 1;
					cache[setIndex][i].tag = subString(binary, 0, tagLen);
					cache[setIndex][i].ageFreq = counter;
					if(policy == 0){
						counter++;
					}
					goto LOOP;
				}
			}

			repIndex = 0;
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(cache[setIndex][i].ageFreq <= cache[setIndex][repIndex].ageFreq){
						repIndex = i;
					}
				}
			}
			cache[setIndex][repIndex].valid = 1;
			cache[setIndex][repIndex].tag = subString(binary, 0, tagLen);
			cache[setIndex][repIndex].ageFreq = counter;
			counter++;

		}else if(strcmp(rw, "W") == 0){
			//printf("W %s - %ld\n", mem, lc);
			writesM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(strncmp(cache[setIndex][i].tag, subString(binary, 0, tagLen), tagLen) == 0){
						hitsC++;
						if(policy == 1){
							cache[setIndex][i].ageFreq = counter;
							counter++;
						}
						goto LOOP;
					}
				}
			}

			missesC++;
			readsM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 0){
					cache[setIndex][i].valid = 1;
					cache[setIndex][i].tag = subString(binary, 0, tagLen);
					cache[setIndex][i].ageFreq = counter;
					if(policy == 0){
						counter++;
					}
					goto LOOP;
				}
			}

			repIndex = 0;
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(cache[setIndex][i].ageFreq <= cache[setIndex][repIndex].ageFreq){
						repIndex = i;
					}
				}
			}
			cache[setIndex][repIndex].valid = 1;
			cache[setIndex][repIndex].tag = subString(binary, 0, tagLen);
			cache[setIndex][repIndex].ageFreq = counter;
			counter++;
		}

	}while(strncmp(pc, "#eof", 4) != 0);

	//Print statements post no-prefetch cache run
	printf("no-prefetch\n");
	printf("Memory reads: %d\n", readsM);
	printf("Memory writes: %d\n", writesM);
	printf("Cache hits: %d\n", hitsC);
	printf("Cache misses: %d\n", missesC);
	printCache(numSets, numLines);
	//Resetting values for prefetch run     ------------PREFETCHRUN-------------
	readsM = 0;
	writesM = 0;
	hitsC = 0;
	missesC = 0;

	FILE* fp2;
	fp2 = fopen(argv[5], "r");

	free(cache);
	cache = (struct line**)malloc(numSets*sizeof(struct line*));
	for(i = 0; i < numSets; i++){
		cache[i] = (struct line*)malloc(numLines*sizeof(struct line));
	}

	for(i = 0; i < numSets; i++){
		for(j = 0; j < numLines; j++){
			cache[i][j].valid = 0;
		}
	}

	free(pc);
	free(rw);
	free(mem);

	pc = (char*)malloc(69*sizeof(char));
	rw = (char*)malloc(10*sizeof(char));
	mem = (char*)malloc(16*sizeof(char));
	char* binary2;
	char* mem2 = (char*)malloc(14*sizeof(char));

	int setIndex2;
	counter = 0;
	repIndex = 0;


	LOOP2:do{
		fscanf(fp2, "%s: ", pc);
		fscanf(fp2, "%s ", rw);
		fscanf(fp2, "%s\n", mem);

		if(strncmp(pc, "#eof", 4) == 0){
			break;
		}

		binary = toBinary(mem);
		setIndex = toDec(subString(binary, tagLen, (48-blockBits)));
		mem2 = prefetchCalc(mem, blockSize/8);
		binary2 = toBinary(mem2);
		setIndex2 = toDec(subString(binary2, tagLen, (48-blockBits)));

		if(strcmp(rw, "R") == 0){
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(strncmp(cache[setIndex][i].tag, subString(binary, 0, tagLen), tagLen) == 0){
						hitsC++;
						if(policy == 1){
							cache[setIndex][i].ageFreq = counter;
							counter++;
						}
						goto LOOP2;
					}
				}
			}

			missesC++;
			readsM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 0){
					cache[setIndex][i].valid = 1;
					cache[setIndex][i].tag = subString(binary, 0, tagLen);
					cache[setIndex][i].ageFreq = counter;
					if(policy == 0){
						counter++;
					}
					counter = prefetch(counter, tagLen, binary2, numLines, setIndex2);
					goto LOOP2;
				}
			}

			repIndex = 0;
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(cache[setIndex][i].ageFreq <= cache[setIndex][repIndex].ageFreq){
						repIndex = i;
					}
				}
			}

			cache[setIndex][repIndex].valid = 1;
			cache[setIndex][repIndex].tag = subString(binary, 0, tagLen);
			cache[setIndex][repIndex].ageFreq = counter;
			counter++;
			counter = prefetch(counter, tagLen, binary2, numLines, setIndex2);

		}else if(strcmp(rw, "W") == 0){
			writesM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(strncmp(cache[setIndex][i].tag, subString(binary, 0, tagLen), tagLen) == 0){
						hitsC++;
						if(policy == 1){
							cache[setIndex][i].ageFreq = counter;
							counter++;
						}
						goto LOOP2;
					}
				}
			}

			missesC++;
			readsM++;

			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 0){
					cache[setIndex][i].valid = 1;
					cache[setIndex][i].tag = subString(binary, 0, tagLen);
					cache[setIndex][i].ageFreq = counter;
					if(policy == 0){
						counter++;
					}
					counter = prefetch(counter, tagLen, binary2, numLines, setIndex2);
					goto LOOP2;
				}
			}

			repIndex = 0;
			for(i = 0; i < numLines; i++){
				if(cache[setIndex][i].valid == 1){
					if(cache[setIndex][i].ageFreq <= cache[setIndex][repIndex].ageFreq){
						repIndex = i;
					}
				}
			}

			cache[setIndex][repIndex].valid = 1;
			cache[setIndex][repIndex].tag = subString(binary, 0, tagLen);
			cache[setIndex][repIndex].ageFreq = counter;
			counter++;
			counter = prefetch(counter, tagLen, binary2, numLines, setIndex2);
		}
	}while(strncmp(pc, "#eof", 4) != 0);

	//Print statements post prefetch cache run
	printf("with-prefetch\n");
	printf("Memory reads: %d\n", readsM);
	printf("Memory writes: %d\n", writesM);
	printf("Cache hits: %d\n", hitsC);
	printf("Cache misses: %d\n", missesC);

	return 0;
}

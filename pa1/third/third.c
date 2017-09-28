#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int data;
	struct Node * next;
} Node;

void Insert(int len);     // Returns node before one you want
void FreeTable();
void Operate(char op, int val);
void Insert(int val);
void ReadFile(int argc, char** argv);  // Reads and inserts or deletes
Node **table = NULL;


int main(int argc, char** argv)
{
	 if(argc != 2)
	 {
	  printf("error");
	  exit(0);
	 }

	 ReadFile(argc, argv);

	 FreeTable();

}

void ReadFile(int argc, char** argv)
{
 int r, val;
 char op;

    FILE* fp = NULL;
    char* filename = NULL;

    filename = argv[1];

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("error: File does not exist\n");
        exit(0);
    }

    r = fscanf(fp, "%c\t", &op);     				// Get operation
    if(r != 1){printf("0\n"); exit(0);}  			// Error throw

    table = (Node**) malloc(10000 * sizeof(Node*));

    while(r != EOF)
    {
    	fscanf(fp, "%d\n", &val);

    	Operate(op, val);
    	r = fscanf(fp, "%c\t", &op);

    }
    fclose(fp);

}

void Operate(char op, int val)
{

	switch(op)
	{
	case 'i':
		Insert(val);
		return;
	case 's':
		Search(val);
		return;
	}

}

void Insert(int val)
{
	//printf("\nInsert: %d", val);
	int index;
	index = val % 10000;
	//printf("Index: %d\n", index);

	Node * find = table[index];
	Node * prev = NULL;

	// Case 1: If the value has not been hashed
	if(!find)
	{
		Node* insert = malloc(sizeof(Node*));
		insert->next = NULL;
		insert->data = val;
		table[index] = insert;
		printf("inserted\n");
		//printf("\tdata: %d\tval: %d\n", table[index]->data, val);

	}
	// Case 2: Collision, so traverse the linked list
	else
	{
		Node* insert = malloc(sizeof(Node*));
		while(find != NULL)
		{
			// Case A: Duplicate
			if(find->data == val)
			{
				printf("duplicate\n");
				return;
			}
			prev = find;
			find = find->next;
		}
		insert->data = val;
		prev->next = insert;
		printf("inserted\n");
		//printf("\tNot in list, inserting at end\n");

	}

}

void Search(int val)
{
	//printf("\nSearch: %d", val);

	int index;
	index = val % 10000;

	// Case 1: key unsued
	if(table[index] == NULL)
	{
		printf("absent\n");
	}
	// Case 2: Search list
	else
	{
		Node * temp = table[index];
		while(temp)
		{
			if(temp->data == val)
			{
				printf("present\n");
				return;
			}
			temp = temp->next;
		}

	}
}



void FreeTable()
{

	int index;
	Node* temp = NULL;
	Node* prev;

	for(index = 0; index <= 10000; index++)
	{
		if(table[index])
		{
			while(temp)
			{
				prev = temp;
				temp = temp->next;
				free(prev);
			}
		}
	}

	free(table);
}

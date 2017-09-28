#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int data;
	struct Node * next;
} Node;

void PrintList();
void Insert(int input);
void Delete(int input);
Node* Find(int input); 				// Returns node before one you want
void FreeList();
void ReadFile(int argc, char** argv);		// Reads and inserts or deletes

Node *head = NULL;


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("error\n");
		exit(0);
	}
	ReadFile(argc, argv);

	PrintList();
	FreeList();

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

    r = fscanf(fp, "%c\t", &op);					// Get operation
    if(r != 1){printf("0\n"); exit(0);}		// Error throw

    r = fscanf(fp, "%d\t", &val);					// Get value to operate

    while(r != EOF)
    {
    	switch(op)
    	{
    	case 'i' :
    		//printf("Insert %d\n", val);
    		Insert(val);
    		break;
    	case 'd' :
    		//printf("Delete %d\n", val);
			Delete(val);
			break;
    	}

    	PrintList();
    	printf("\n");
    	r = fscanf(fp, "%c\t", &op);		//scan now for next round of operations
    	fscanf(fp, "%d\n", &val);
    }

}

void Insert(int input)
{
	printf("INSERTING: %d\t", input);

	Node* insert = malloc(sizeof(Node*));
	insert->data = input;
	insert->next = NULL;
	Node* trailing;

	// Case 1: NULL List

	if( head == NULL)
	{
		printf("CASE 1\n");
		head = insert;
		head->next = NULL;
	}
	// Case 2: Insert @ beginning
	else if (head->data > input)
	{
		printf("CASE 2\n");
		insert->next = head;
		head = insert;
	}
	// Case 3: Traverse
	else
	{
		printf("CASE 3\n");
		trailing = Find(input);
		insert->next = trailing->next;
		trailing->next = insert;

	}



}

void Delete(int input)
{
	printf("DELETING: %d\t", input);

	Node* trailing;

	// Case 1: NULL List

	if( head == NULL)
	{
		printf("CASE 1\n");
	}
	// Case 3: Delete head
	else if (head->data == input)
	{
		printf("CASE 3\n");
		head = head->next;
	}
	// Case 4: Traverse
	else
	{
		printf("CASE 4\n");
		trailing = Find(input);
		if(trailing == NULL) return;
		//newNext = trailing->next->next;
		trailing->next = trailing->next->next;
	}

}

void PrintList()
{

	Node* temp = head;
	printf("LIST: ");

	while(temp != NULL)
	{
		printf("%d ", temp->data);
		temp = temp->next;
	}
	printf("\n");

}

void FreeList()
{

	Node* temp = head;
	Node* trailing = head;

		while(trailing != NULL)
		{
			temp = temp->next;
			free(trailing);
			trailing = temp;
		}
}

Node* Find(int input)
{
	Node* target;
	Node* trailing;
	target = head;
	trailing = NULL;

	while(target)
	{
		if(target->data >= input) return trailing;
		trailing = target;
		target = target->next;
	}


	return trailing;
}


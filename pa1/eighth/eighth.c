#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int data;
	struct Node * left;
	struct Node * right;
} Node;

void Insert(int len);     // Returns node before one you want
void FreeTable();
void Operate(char op, int val);
void Insert(int val);
void Search(int val);
void ReadFile(int argc, char** argv);  // Reads and inserts or deletes
Node *root = NULL;



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
	Node * insert = NULL;

	// Case 1: Empty tree
	if(!root)
	{
		insert = (Node*) malloc(sizeof(Node));
		root = insert;
		printf("inserted %d", val);
		return;
	}
	else
	{
		Node * temp = root;
		while(root){
			// Val is smaller, go left
			if(root->data > val)
			{
				if(root->left)
				{
					root = root->left;
					continue;
				}
				temp = (Node*) malloc(sizeof(Node));
				temp->data = val;
				root->left = temp;
				printf("inserted %d", val);
			}
			// Val is bigger, go right
			else if(root->data < val)
			{
				if(root->right)
				{
					root = root->right;
					continue;
				}
				temp = (Node*) malloc(sizeof(Node));
				temp->data = val;
				root->right = temp;
				printf("inserted %d\n", val);
			}
			// Val is present, print duplicate
			else if(root->data == val)
			{
				printf("duplicate\n");
			}
		}
	}

}

void Search(int val)
{
	Node * insert = NULL;

		// Case 1: Empty tree
		if(!root)
		{
			printf("absent %d", val);
			return;
		}
		else
		{
			Node * temp = root;
			while(root){
				// Val is smaller, go left
				if(root->data > val)
				{
					root = root->left;
					continue;
				}
				// Val is bigger, go right
				else if(root->data < val)
				{
					root = root->right;
					continue;
				}
				// Val is present, print duplicate
				else if(root->data == val)
				{
					printf("present %d", val);
					return;
				}
			}
		}
		printf("absent %d", val);
}



void FreeTable()
{

}

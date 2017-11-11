#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int data;
	struct Node * left;
	struct Node * right;
} Node;

void Insert(int len);     // Returns node before one you want
void FreeTree(Node* temp);
void Operate(char op, int val);
void Insert(int val);
void Search(int val);
void ReadFile(int argc, char** argv);  // Reads and inserts or deletes
Node * MakeNode(int val);
Node *root = NULL;



int main(int argc, char** argv)
{
	 if(argc != 2)
	 {
	  printf("error");
	  exit(0);
	 }

	 ReadFile(argc, argv);

	 FreeTree(root);
	 return 0;

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
	//Node * insert = NULL;
	int cnt;
	cnt = 1;
	// Case 1: Empty tree
	if(root == NULL)
	{
		Node * insert = (Node*)malloc(sizeof(Node));
		insert = MakeNode(val);
		root = insert;
		printf("inserted 1\n");
		return;
	}
	else
	{
		Node * temp = root;

		while(temp){
			// Val is smaller, go left
			if(temp->data > val)
			{
				if(temp->left != NULL)
				{
					temp = temp->left;
					cnt++;
					continue;
				}
				Node * insert = (Node*) malloc(sizeof(Node));
				insert = MakeNode(val);;
				temp->left = insert;
				cnt++;
				printf("inserted %d\n", cnt);
				return;
			}
			// Val is bigger, go right
			else if(temp->data < val)
			{
				if(temp->right != NULL)
				{
					temp = temp->right;
					cnt++;
					continue;
				}
				Node * insert = (Node*) malloc(sizeof(Node));
				insert = MakeNode(val);
				temp->right = insert;
				cnt++;
				printf("inserted %d\n", cnt);
				return;
			}
			// Val is present, print duplicate
			else if(temp->data == val)
			{
				printf("duplicate\n");
				return;
			}
		}
	}

}

void Search(int val)
{
	int cnt;
	cnt = 1;
		// Case 1: Empty tree
	if(root == NULL)
	{
		printf("absent %d\n", val);
		return;
	}
	else
	{
		Node * temp = root;
			while(temp != NULL){
			// Val is smaller, go left
			if(temp->data > val)
			{
				temp = temp->left;
				cnt++;
				continue;
			}
			// Val is bigger, go right
			else if(temp->data < val)
			{
				temp = temp->right;
				cnt++;
				continue;
			}
			// Val is present, print duplicate
			else if(temp->data == val)
			{
				printf("present %d\n", cnt);
				return;
			}
		}
	}
	printf("absent\n");
}

Node * MakeNode(int val)
{
	Node * newNode = (Node*) malloc(sizeof(Node));
	newNode->data = val;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

void FreeTree(Node* temp)
{
	if(temp == NULL) return;
	FreeTree(temp->left);
	FreeTree(temp->right);
	free(temp);
}
